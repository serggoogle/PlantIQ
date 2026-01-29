import com.plantiq.sensors.Sensor;
import com.plantiq.simulator.SensorToJsonMapper;
import org.junit.jupiter.api.Test;
public class SensorToJsonMapperTest {

    @Test
    public void testSensorToJsonMapping() throws Exception {
        String sensorName = "testSensor";
        String deviceId = "testDevice";
        double sensorValue = 1234.1234;
        Sensor sensor = new Sensor(sensorName,deviceId);
        SensorToJsonMapper sensorToJsonMapper = new SensorToJsonMapper(sensor);
        String json = sensorToJsonMapper.map(sensorValue);

        String expectedJson = "{\"sensor\":\""+ sensorName +"\",\"deviceId\":\""+ deviceId +"\",\"value\":" + sensorValue + "}";
        assert json.equals(expectedJson) : "JSON mapping failed";
    }
}