import com.plantiq.sensors.Sensor;
import com.plantiq.simulator.SensorToJsonMapper;
import org.junit.jupiter.api.Test;
public class SensorToJsonMapperTest {

    @Test
    public void testSensorToJsonMapping() throws Exception {
        Sensor sensor = new Sensor("testSensor","testDevice");
        double sensorValue = 1234.1234;
        SensorToJsonMapper sensorToJsonMapper = new SensorToJsonMapper(sensor);
        String json = sensorToJsonMapper.map(sensorValue);

        System.out.println(json);
        String expectedJson = "{\"sensor\":\"testSensor\",\"deviceId\":\"testDevice\",\"value\":" + "" + "}";
        assert json.equals(expectedJson) : "JSON mapping failed";
    }
}