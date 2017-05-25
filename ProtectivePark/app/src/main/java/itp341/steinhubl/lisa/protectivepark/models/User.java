package itp341.steinhubl.lisa.protectivepark.models;

import java.io.Serializable;

import itp341.steinhubl.lisa.protectivepark.ParkingSpot;

/**
 * Created by lsteinhubl on 5/7/17.
 */

public class User implements Serializable {

    String email;
    String emergencyContactName;
    String emergencyContactNumber;
    ParkingSpot parkingSpot;

    public User() {

    }

    public void setEmail(String email) {this.email = email; }
    public String getEmail() {return email;}
    public void setEmergencyContactName(String emergencyContactName) {this.emergencyContactName = emergencyContactName;}
    public String getEmergencyContactName() {return emergencyContactName;}
    public void setEmergencyContactNumber(String number) {this.emergencyContactNumber = number;}
    public String getEmergencyContactNumber() {return emergencyContactNumber;}
    public void setParkingSpot(ParkingSpot p) {this.parkingSpot = p;}
    public ParkingSpot getParkingSpot() {return parkingSpot;}

}
