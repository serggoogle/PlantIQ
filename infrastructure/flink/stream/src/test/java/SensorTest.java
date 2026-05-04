import com.plantiq.sensors.*;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class SensorTest {
    private static String DEVICE_ID = "testDevice";
    private static String PLANT_NAME = "testPlant";
    private static Long MIN_THRESHOLD = 0L;
    private static Long MAX_THRESHOLD = 100_000_000_000L;
    @Test
    public void testDefaultTemperatureSensor() throws Exception {
        String sensorName = "temperature_sensor";
        TemperatureSensor sensor = new TemperatureSensor(DEVICE_ID, PLANT_NAME, sensorName,true);
        assertEquals(sensor.getSensorName(), sensorName);
        assertEquals(sensor.getDeviceId(), DEVICE_ID);
        assertEquals(sensor.getPlantName(), (PLANT_NAME));
    }

    @Test
    public void testDefaultMoistureSensor() throws Exception {
        final String sensorName = "moisture_sensor";
        MoistureSensor sensor = new MoistureSensor(DEVICE_ID, PLANT_NAME, sensorName);
        sensor.setMinThreshold(MIN_THRESHOLD);
        sensor.setMaxThreshold(MAX_THRESHOLD);
        assertEquals(sensorName, sensor.getSensorName());
        assertEquals(sensor.getDeviceId(), DEVICE_ID);
        assertEquals(sensor.getPlantName(), (PLANT_NAME));
    }

}
