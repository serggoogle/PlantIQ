package com.plantiq.sensors;
import java.io.Serializable;

public class Sensor implements Serializable {
    private String name = "";
    private String labelName = "";
    private String labelValue = "";
    private double minThreshold = 0.0;
    private double maxThreshold = 0.0;

    Sensor(String name){
        this.name = name;
    }

    public String getName(){
        return this.name;
    }

    public String getLabelName(){
        return this.labelName;
    }

    public String getLabelValue(){
        return this.labelValue;
    }

    public void setMinThreshold(double threshold){
        this.minThreshold = threshold;
    }

    public void setMaxThreshold(double threshold){
       this.maxThreshold = threshold;
    }

    public double getMinThreshold(){
        return minThreshold;
    }

    public double getMaxThreshold(){
        return maxThreshold;
    }
}
