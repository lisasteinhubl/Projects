package itp341.steinhubl.lisa.protectivepark;

import android.*;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.os.CountDownTimer;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.content.ContextCompat;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.telephony.SmsManager;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AbsListView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.PopupWindow;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.io.IOException;
import java.util.List;

import itp341.steinhubl.lisa.protectivepark.models.User;

public class MapsActivity extends AppCompatActivity implements OnMapReadyCallback,
        GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    private static final int MY_PERMISSIONS_REQUEST_LOCATION = 0;
    private GoogleMap mMap;
    private LatLng currentCameraOrZoomLatLng;
    private Marker addSpaceMarker;
    private DatabaseReference mDatabase;
    Button ReturnToSpot;
    FirebaseUser currUserFirebase;
    User currUser;
    String userId;
    GoogleApiClient mGoogleApiClient;
    Location mLastLocation;
    float distanceToSpot;
    float timeToSpot;
    TextView timeToCar;
    private MalibuCountDownTimer countDownTimer;
    Button alertEmergencyContact;
    private static final int MY_PERMISSIONS_REQUEST_SEND_SMS =0;
    String emergencyContactNumber = "";
    String emergencyMessage = "";

    Button stopTimerButton;
    boolean safeFinish = false;
    String garageLevel;
    String spotNumber;
    boolean userClickedSave = true;



    @Override
    public void onConnected(Bundle connectionHint) {
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED &&
                ActivityCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_COARSE_LOCATION)
                        != PackageManager.PERMISSION_GRANTED) {

            //Request premissions
            ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION},
                    MY_PERMISSIONS_REQUEST_LOCATION);

            return;
        }
        mLastLocation = LocationServices.FusedLocationApi.getLastLocation(
                mGoogleApiClient);

    }

    @Override
    public void onConnectionSuspended(int i) {

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) { switch(item.getItemId()) {

//        case R.id.addEmergencyContact:
//            showContactDialog();
//            //add the function to perform here
//            return(true);
        case R.id.logoutMenuButton:
            FirebaseAuth.getInstance().signOut();
            Intent i = new Intent(MapsActivity.this, LoginActivity.class);
            MapsActivity.this.startActivity(i);
            //add the function to perform here
            return(true);
    }
        return(super.onOptionsItemSelected(item));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
        currUserFirebase = FirebaseAuth.getInstance().getCurrentUser();

        distanceToSpot = 0.0f;



        // Create an instance of GoogleAPIClient.
        if (mGoogleApiClient == null) {
            mGoogleApiClient = new GoogleApiClient.Builder(this)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .addApi(LocationServices.API)
                    .build();
        }

        stopTimerButton = (Button) findViewById(R.id.stopTimerButton);
        stopTimerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (countDownTimer != null) {
                    safeFinish = true;
                    countDownTimer.onFinish();
                }
                else {
                    return;
                }
            }
        });

    }

    protected void onStart() {
        mGoogleApiClient.connect();
        super.onStart();
    }

    protected void onStop() {
        mGoogleApiClient.disconnect();
        super.onStop();
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        currentCameraOrZoomLatLng = mMap.getCameraPosition().target;

        mDatabase = FirebaseDatabase.getInstance().getReference("users");
        currUserFirebase = FirebaseAuth.getInstance().getCurrentUser();
        //mDatabase.setValue(currUserFirebase.getEmail());
        //mDatabase = FirebaseDatabase.getInstance().getReference(e);

        timeToCar = (TextView) findViewById(R.id.timeToCar);
        ReturnToSpot = (Button) findViewById(R.id.returnToSpotButton);

        //Allow user to go to current location if permission granted
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED &&
                ActivityCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_COARSE_LOCATION)
                        != PackageManager.PERMISSION_GRANTED) {

            //Add permissions
            ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION},
                    MY_PERMISSIONS_REQUEST_LOCATION);

            return;
        }

        alertEmergencyContact = (Button) findViewById(R.id.alertContactButton);



        mMap.setMyLocationEnabled(true);

        mMap.setOnMyLocationButtonClickListener(new GoogleMap.OnMyLocationButtonClickListener() {
            @Override
            public boolean onMyLocationButtonClick() {
                if (mLastLocation == null) {
                    mLastLocation = mMap.getMyLocation();
                }
                LatLng loc = new LatLng(mLastLocation.getLatitude(),mLastLocation.getLongitude());
                mMap.moveCamera(CameraUpdateFactory.newLatLng(loc));
                mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(loc, 15));

                return true;
            }
        });



        mMap.setOnMapLongClickListener(new GoogleMap.OnMapLongClickListener() {
            @Override
            public void onMapLongClick(LatLng latLng) {
                if (addSpaceMarker == null) {
                    MarkerOptions marker = new MarkerOptions();
                    marker.position(latLng);
                    marker.title("New Spot");
                    marker.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE));
                    addSpaceMarker = mMap.addMarker(marker);
                }

            }
        });


        mMap.setOnMarkerClickListener(new GoogleMap.OnMarkerClickListener() {
            @Override
            public boolean onMarkerClick(Marker marker) {

                showAddDialog();

                if(userClickedSave) {
                    double lat = marker.getPosition().latitude;
                    double longitude = marker.getPosition().longitude;
                    ParkingSpot ps = new ParkingSpot();
                    ps.SetLongLad(longitude, lat);

                    //Allow the user to now see the other buttons
                    ReturnToSpot.setClickable(true);
                    ReturnToSpot.setEnabled(true);
                    ReturnToSpot.setVisibility(View.VISIBLE);
                    timeToCar.setVisibility(View.VISIBLE);
                    stopTimerButton.setVisibility(View.VISIBLE);
                    alertEmergencyContact.setVisibility(View.VISIBLE);

                    if (spotNumber != null) {
                        ps.SetParkingSpotNumber(Integer.parseInt(spotNumber));
                    }
                    if (garageLevel != null) {
                        ps.SetGarageLevel(Integer.parseInt(garageLevel));
                    }
                    userId = mDatabase.push().getKey();
                    mDatabase.child(userId).setValue(ps);
                    userClickedSave = false;
                }

                return false;
            }
        });


        //What happens when the user wants to go back to their spot
        ReturnToSpot.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //Read in longitude and latitude from database
                final Location targetLocation = new Location("");
                DatabaseReference spotRef = mDatabase.child(userId);
                spotRef.addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        ParkingSpot ps = dataSnapshot.getValue(ParkingSpot.class);
                        double longitude = ps.GetLong();
                        double latitude = ps.GetLat();
                        targetLocation.setLatitude(latitude);
                        targetLocation.setLongitude(longitude);
                        if (mLastLocation == null) {
                            mLastLocation = mMap.getMyLocation();
                        }
                        timeToCar.setVisibility(View.VISIBLE);
                        distanceToSpot = targetLocation.distanceTo(mLastLocation);
                        distanceToSpot /= 1000;
                        timeToSpot = distanceToSpot * 10;

                        countDownTimer = new MalibuCountDownTimer((long)timeToSpot*60000, 500);
                        countDownTimer.start();


                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {

                    }
                });


            }
        });

        alertEmergencyContact.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (emergencyContactNumber.equals("")) {
                    showContactDialog();
                }
                else {
                    sendSMSMessage();
                }
            }
        });

    }


    //This allows search by both address and long/lat
    public void onMapSearch(View view) {
        EditText locationSearch = (EditText) findViewById(R.id.editText);
        //No text entered to search bar
        if(locationSearch.getText().toString().equals("")){
            return;
        }
        String location = locationSearch.getText().toString();
        List<Address> addressList = null;
        Geocoder geocoder;
        if (location != null || !location.equals("")) {
            geocoder = new Geocoder(this);
            try {
                addressList = geocoder.getFromLocationName(location, 1);

            } catch (IOException e) {
                e.printStackTrace();
            }
            Address address = addressList.get(0);
            LatLng latLng = new LatLng(address.getLatitude(), address.getLongitude());
            mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng, 15));
        }
    }

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {

    }

    protected void sendSMSMessage() {
        emergencyMessage = currUserFirebase.getEmail() + getResources().getString(R.string.EmergencyMessage);

        if (mLastLocation != null) {
            emergencyMessage += getResources().getString(R.string.AtLong) + mLastLocation.getLongitude() +
                    getResources().getString(R.string.AndLat) + mLastLocation.getLatitude();
        }

        if (!spotNumber.equals("")) {
            emergencyMessage += getResources().getString(R.string.TheyAreAtSpot) + spotNumber;
        }
        if (!garageLevel.equals("")) {
            emergencyMessage += getResources().getString(R.string.TheyAreOnLevel) + garageLevel;
        }

        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.SEND_SMS)
                != PackageManager.PERMISSION_GRANTED) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    android.Manifest.permission.SEND_SMS)) {
            } else {
                ActivityCompat.requestPermissions(this,
                        new String[]{android.Manifest.permission.SEND_SMS},
                        MY_PERMISSIONS_REQUEST_SEND_SMS);
            }
        }
        else {
            SmsManager smsManager = SmsManager.getDefault();
            smsManager.sendTextMessage(emergencyContactNumber, null, emergencyMessage, null, null);
            Toast.makeText(getApplicationContext(), "SMS sent, message: " + emergencyMessage,
                    Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,String permissions[], int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_SEND_SMS: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    SmsManager smsManager = SmsManager.getDefault();
                    smsManager.sendTextMessage(emergencyContactNumber, null, emergencyMessage, null, null);
                    Toast.makeText(getApplicationContext(), "SMS sent.",
                            Toast.LENGTH_LONG).show();
                } else {
                    Toast.makeText(getApplicationContext(),
                            "SMS failed, please try again.", Toast.LENGTH_LONG).show();
                    return;
                }
            }
        }

    }


    //Timer that starts when user goes back to their car
    public class MalibuCountDownTimer extends CountDownTimer
    {

        public MalibuCountDownTimer(long startTime, long interval)
        {
            super(startTime, interval);
        }

        @Override
        public void onFinish()
        {
            timeToCar.setText("Time's up!");
            countDownTimer.cancel();
            if (safeFinish == false) {
                if (!emergencyContactNumber.equals("")) {
                    sendSMSMessage();
                }
            }
            timeToCar.setText("Please add contact");
        }

        @Override
        public void onTick(long millisUntilFinished)
        {

            long seconds = millisUntilFinished / 1000;

            timeToCar.setText("Time to Car: "+String.format("%02d:%02d:%02d", seconds / 3600,
                    (seconds % 3600) / 60, (seconds % 60)));

        }
    }


    private void showAddDialog() {

        LayoutInflater inflater = LayoutInflater.from(MapsActivity.this);
        final View CustomView = inflater.inflate(R.layout.fragment_add_spot_dialog, null);

        final TextView spotNumberText = (EditText) CustomView.findViewById(R.id.SpotNumberEditText);
        final TextView garageLevelText = (EditText) CustomView.findViewById(R.id.GarageLevelEditText);
        AlertDialog dialog = new AlertDialog.Builder(MapsActivity.this)
                .setView(CustomView)
                .setPositiveButton("Save", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        spotNumber = spotNumberText.getText().toString();
                        garageLevel = garageLevelText.getText().toString();
                        userClickedSave = false;
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        addSpaceMarker = null;
                        userClickedSave = true;
                    }
                }).create();
        dialog.show();

    }

    private void showContactDialog() {

        LayoutInflater inflater = LayoutInflater.from(MapsActivity.this);
        final View CustomView = inflater.inflate(R.layout.fragment_add_contact, null);

        final TextView nameText = (EditText) CustomView.findViewById(R.id.ContactNameEditText);
        final TextView numberText = (EditText) CustomView.findViewById(R.id.PhoneEditText);
        AlertDialog dialog = new AlertDialog.Builder(MapsActivity.this)
                .setView(CustomView)
                .setPositiveButton("Save", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        emergencyContactNumber = numberText.getText().toString();
                        emergencyMessage = currUserFirebase.getDisplayName() + R.string.EmergencyMessage;
                        mDatabase.child(userId).setValue(emergencyContactNumber);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        addSpaceMarker = null;
                        userClickedSave = true;
                    }
                }).create();
        dialog.show();

    }




}



