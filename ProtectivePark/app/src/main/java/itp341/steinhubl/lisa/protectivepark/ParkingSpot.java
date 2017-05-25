package itp341.steinhubl.lisa.protectivepark;

import java.io.Serializable;

/**
 * Created by lsteinhubl on 4/27/17.
 */

public class ParkingSpot implements Serializable {

    String UserName;
    int GarageLevel;
    int ParkingSpotNumber;
    double Longitude;
    double Latitude;


    public ParkingSpot() {

    }

    public void SetUserName(String name) {
        UserName = name;
    }

    public String GetUserName() {
        return UserName;
    }

    public void SetGarageLevel(int level) {
        GarageLevel = level;
    }

    public int GetGarageLevel() {
        return GarageLevel;
    }

    public void SetParkingSpotNumber(int number) {
        ParkingSpotNumber = number;
    }

    public int GetParkingSpotNumber() {
        return ParkingSpotNumber;
    }

    public void SetLongLad(double longi, double lati) {
        this.Longitude = longi;
        this.Latitude = lati;
    }

    public double GetLong() {
        return  Longitude;
    }

    public double GetLat() {
        return Latitude;
    }

}
