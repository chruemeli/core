import webpackMockServer from "webpack-mock-server";
import nodePath from "path";

const mockSensorData = [
    {
        name: "room_humidity",
        type: "humidity",
        value: 50.1
    }, 
    {
        name: "room_temperature",
        type: "temperature", 
        value: 21.8
    }, 
    {
        name: "soil_moisture",
        type: "humidity", 
        value: 69
    }
];

const mockWifis = [
    {
        bssid: "AA:BB:CC:DD:EE:FF",
        ssid: "Swisscom",
        rssi: -74,
        channel: 3,
        secure: 4
    },
    {
        bssid: "AA:BB:CC:DD:EE:FF",
        ssid: "Init 7",
        rssi: -77,
        channel: 1,
        secure: 7
    },
    {
        bssid: "AA:BB:CC:DD:EE:FF",
        ssid: "Sunrise",
        rssi: -71,
        channel: 5,
        secure: 4
    },
    {
        bssid: "AA:BB:CC:DD:EE:FF",
        ssid: "Salt",
        rssi: -91,
        channel: 11,
        secure: 4
    }
];

// app is expressjs application
export default webpackMockServer.add((app, helper) => {
    app.get("/api/config", (req, res) => {
        res.json({
            name: "iot-aabbcc",
            isCaptivePortalEnabled: true
        });
    });

    app.get("/api/sensor/:sensorName", (req, res) => {
        let sensorName = req.params["sensorName"];

        if(mockSensorData[sensorName] !== undefined) {
            res.json(mockSensorData[sensorName]);
        }

        res.json({});
    });
        
    app.get("/api/sensor", (req, res) => {
        res.json(mockSensorData);
    });

    app.get("/api/wifi", (req, res) => {
        res.json(mockWifis);
    });
});