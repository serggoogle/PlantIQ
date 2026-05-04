package com.plantiq;

import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonCreator;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonProperty;

import java.io.Serializable;
import java.util.Objects;

public class Plant implements Serializable {

    private String deviceId;
    private String plantName;
    private String type;
    private String species;

    Plant() {}
    @JsonCreator(mode = JsonCreator.Mode.PROPERTIES)
    public Plant(
            @JsonProperty("deviceId") String deviceId,
            @JsonProperty("plant_name") String plantName,
            @JsonProperty("plant_type") String type,
            @JsonProperty("plant_species") String species
    )
    {
        this.deviceId = deviceId;
        this.plantName = plantName;
        this.type = type;
        this.species = species;
    }
    @JsonProperty("deviceId")
    public String getDeviceId() {
        return this.deviceId;
    }

    @JsonProperty("plant_name")
    public String getPlantName() {
        return this.plantName;
    }
    @JsonProperty("plant_type")
    public String getType() {
        return this.type;
    }

    @JsonProperty("plant_species")
    public String getSpecies() {
        return this.species;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId;
    }

    public void setPlantName(String plantName) {
        this.plantName = (plantName != null ? plantName : "");
    }

    public void setType(String type) {

        this.type = (type != null ? type : "");
    }

    public void setSpecies(String species) {
        this.species = species;
    }

    @Override
    public boolean equals(Object o) {

        if (this == o)
            return true;
        if (!(o instanceof Plant plant))
            return false;
        return Objects.equals(this.deviceId, plant.deviceId) && Objects.equals(this.plantName, plant.plantName)
                && Objects.equals(this.type, plant.type)
                && Objects.equals(this.species, plant.species);
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.deviceId, this.plantName, this.type, this.species);
    }

    @Override
    public String toString() {
        return "Plant{" + "id=" + this.deviceId + ", name='" + this.plantName + '\'' + ", type='" + this.type + '\'' + ", species='" + this.species + '\'' + '}';
    }
}