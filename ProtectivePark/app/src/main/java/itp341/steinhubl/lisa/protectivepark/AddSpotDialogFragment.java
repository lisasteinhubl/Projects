package itp341.steinhubl.lisa.protectivepark;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.support.v4.app.DialogFragment;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class AddSpotDialogFragment extends DialogFragment {

    String garageLevel;
    String spotNumber;
    EditText spotNumberEdit;
    EditText garageLevelEdit;
    Button saveButton;
    public Button cancelButton;
    View v;




    public AddSpotDialogFragment() {
        // Required empty public constructor
    }


    public static AddSpotDialogFragment newInstance() {
        AddSpotDialogFragment fragment = new AddSpotDialogFragment();
        Bundle args = new Bundle();

        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {

        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        v = inflater.inflate(R.layout.fragment_add_spot_dialog, container, false);

//        spotNumberEdit = (EditText) v.findViewById(R.id.SpotNumberEditText);
//        garageLevelEdit = (EditText) v.findViewById(R.id.GarageLevelEditText);
//        saveButton = (Button) v.findViewById(R.id.SaveButton);
//        cancelButton = (Button) v.findViewById(R.id.CancelSpotButton);

        getDialog().getWindow().setSoftInputMode(
                WindowManager.LayoutParams.SOFT_INPUT_STATE_VISIBLE);

//        cancelButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                dismiss();
//            }
//        });
//
//        saveButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                garageLevel = garageLevelEdit.getText().toString();
//                spotNumber = spotNumberEdit.getText().toString();
//                ParkingSpot parkingSpot = new ParkingSpot();
//                parkingSpot.SetGarageLevel(Integer.parseInt(garageLevel));
//                parkingSpot.SetParkingSpotNumber(Integer.parseInt(spotNumber));
//
//                Bundle bundle = new Bundle();
//                bundle.putSerializable("NewSpot", parkingSpot);
//
//
//            }
//        });



        return v;
    }




}