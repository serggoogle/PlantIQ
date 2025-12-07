# Plant Monitoring Platform 🍃🌿☘️
> A modernized monitoring system that provides insight to a plants lifecylce 🩺

## Table of Contents
<!-- "## " is H2. Use this section for easy navigation in large READMEs -->
- [Overview](#overview)
- [Installation](#installation)
- [Build](#build)
- [Testing](#build)
- [License](#license)
## Overview


The purpose of this project is to get familiar with the tools mentioned in the Installation section of this document. It will give the reader some insight on how to these systems work together by creating a plant monitoring system.

Each plant will have at two modular sensors which will give the user a insight about the plants moisture & temperature levels.

This project consists of thee major parts:
    
1. Base Code & Infrastructure Setup
2. Use Promethius Alerts to Alert User on Sensor Metrics
3. Automate Plant Irrigation


![High-level System Overview](images/PMS.drawio.svg)


## Installation
### Prerequisites
For local development, it's recommened to have the following tools installed:

Tools for Development & Infrastructure:
* [PlatformIO](https://platformio.org/?utm_source=platformio&utm_medium=piohome) - For Embedded Software Development
* [Docker w/ Docker Compose](https://docs.docker.com/get-started/) - Containerization technology for building and containerizing your applications
    * Used for the local infrastructure

Tools for future experimental software (Kafka, Flink, etc.):
* Java 17 JDK
    * [sdkman](https://sdkman.io) to experiment with differnt java versions.
* [Kubernetes](https://kubernetes.io/docs/tasks/tools/install-kubectl-macos/) - open-source platform for automating the deployment, scaling, and management of containerized applications.
* [Minikube](https://minikube.sigs.k8s.io/docs/start/?arch=%2Fmacos%2Fx86-64%2Fstable%2Fbinary+download) - Local Kubernetes focusing on making it easy to learn and develop for Kubernetes.
* [k9s](https://k9scli.io/) - A terminal based UI to interact with our Kubernetes clusters.

**Clone the repository**
   ```sh
   git clone https://github.com/yourusername/PlantMonitoringPlatform.git
   ```

## Credentials Setup

Before building and running the project, you need to create two configuration files to store your credentials. **Do not commit these files to version control if they contain sensitive information.** These files are included in the .gitignore but feel free to double check.

### 1. MQTT Credentials
Create a file at `lib/MQTT/MQTTCredentials.h` with the following content:
```cpp
#pragma once

#define MQTT_SERVER "your-mqtt-broker-address"
#define MQTT_PORT 1883
#define MQTT_USER "your-username"
#define MQTT_PASSWORD "your-password"
```
Replace the placeholder values with your actual MQTT broker information.

### 2. Prometheus Credentials
Create a file at lib/Prometheus/Config.h with the following content:
```cpp
#pragma once

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"
#define URL "PROMETHEUS-SERVER-IP"
#define PATH "/api/v1/write"
#define PORT 9090
```
Replace the placeholder values with your actual WiFi and Prometheus server details.

## Infrastructure Setup
The `setup.sh` script will create the containers needed for development & testing.
   * **Note:** Make sure docker is running on your machine.
   ```sh
    cd PlantMonitoringPlatform/
    ./setup.sh
   ```
   Give it a minute and check if the containers are up.
   ```sh
    docker ps -a
   ```
Once everything is up, you can access both services:
* Grafana: http://localhost:3000/
    * initial username/password is *admin*
* Prometheus: http://localhost:9090/
* Apache Flink: http://localhost:8081/

You can now exec into your development container to compile code.
```sh
docker exec -it dev-container /bin/bash
# Compile src code
pio run
```

## Infrastructure Taredown
The `taredown.sh` script will give you the option to tare down specific services.
```sh
Tare down infrastructure/metrics-compose.yml?[y/n]: 
Tare down infrastructure/flink-compose.yml?[y/n]: 
Tare down development environment?[y/n]: 
```

## Build
### Emmbedded Software
PlatformIO will be used to compile the C++ code and upload to the Arduino board. We primerly use the `pio` CLI command to build, upload, monitor and clean the project. PlatformIO has an extensive list that documents the usage [here](https://docs.platformio.org/en/latest/core/userguide/index.html#usage), but here's a brief summary of the commands we use and what they do.

```sh
brew install platformio
```

`pio run`
- Compiles (builds) your project and generates the firmware binary for your board.

`pio run -t upload`
- Builds your project (if needed) and uploads (flashes) the compiled firmware to your connected microcontroller board.

`pio run -t monitor`
- Builds your project (if needed) and opens the PlatformIO Serial Monitor to view serial output from your device.

`pio run -t clean`
- Removes all build artifacts (compiled files, firmware binaries, etc.) from your project’s .pio build directory, allowing you to do a fresh rebuild next time you compile.
### Simulation Jar


## Testing
Apache Flink is being used to simulate plant metrics. Currennly this establishes a data pipeline to Prometheus therefore providing metrics visualization on Grafana.

### Simulation
The simulatior will create artificial data and send it to Prometheus. You can define however many simulators you want as long as you have the necessary resources.

Ex. We want to create 2 simulators.
Create a JAR artifact with the latest changes
```sh
docker exec -it dev-container /bin/bash
cd test/sensor-data-generator
mvn clean package
```
```sh
cd test/
./start-sensor-data-generator.sh
nerator(s)
Using ac82073f-89e6-4356-9ef9-004063436880_sensor-data-generator-1.0.jar
Number of running jobs: 
Available taskslots: 2
Current number of taskmanagers: 1
Required number of taskmanagers: 1
Started sensor-data-sim-1a58f5 with jobid ca43f7936621db5f23229fa97c19556e
Started sensor-data-sim-d1edbb with jobid d59a80f917e2d7ceadf86152557e6808
Done
```
