import com.plantiq.sensors.Sensor;
import com.plantiq.simulator.SensorToJsonMapper;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;

public class SensorToJsonMapperTest {
    private static final String EXPECTED_JSON_FILE_PATH = "src/test/resources/TestSensor.json";
    private static String expectedJson;
    private static Sensor testSensor;
    @BeforeAll
    public static void init() throws IOException {
        testSensor = Sensor.TEST_SENSOR();
        testSensor.setTimestamp(0L);
        expectedJson = new ObjectMapper().readTree(new File(EXPECTED_JSON_FILE_PATH)).toString();
    }
    @Test
    public void testSensorToJsonMapping() throws Exception
    {
        double sensorValue = 1234.1234;
        SensorToJsonMapper sensorToJsonMapper = new SensorToJsonMapper(testSensor);
        String actualJson = sensorToJsonMapper.map(sensorValue);

        String errorMessage = new StringBuilder("JSON mapping failed\nExpected:\n")
                .append(expectedJson + "\n")
                .append("Actual:\n")
                .append(actualJson)
                .toString();

        assert actualJson.equals(expectedJson) : errorMessage;
    }
}