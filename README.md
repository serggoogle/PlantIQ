# PlantIQ 🍃🌿☘️
> A modernized monitoring system that provides insight to a plants lifecylce 🩺♻️

## Table of Contents
<!-- "## " is H2. Use this section for easy navigation in large READMEs -->
- [Overview](#overview)
- [Installation](#installation)
- [Build](#build)
- [Testing](#testing)
## Overview


The purpose of this project is to get familiar with the tools mentioned in the Installation section of this document. It will give the reader some insight on how to these systems work together by creating a plant monitoring system.

Each plant will have at two modular sensors which will give the user a insight about the plants moisture & temperature levels.

This project consists of thee major parts:
    
1. Base Code & Infrastructure Setup
2. Use Promethius Alerts to Alert User on Sensor Metrics
3. Automate Plant Irrigation


![High-level System Overview](images/High-Level-Architecture.drawio.svg)


## Installation
### Prerequisites
For local development, it's recommened to have the following tools installed:

Tools for Development & Infrastructure:
* [PlatformIO](https://platformio.org/?utm_source=platformio&utm_medium=piohome) - For Embedded Software Development
* [Docker w/ Docker Compose](https://docs.docker.com/get-started/) - Containerization technology for building and containerizing your applications
    * Used for the local infrastructure & development container

Tools for future experimental software (Kafka, Flink, etc.):
* [Kubernetes](https://kubernetes.io/docs/tasks/tools/install-kubectl-macos/) - open-source platform for automating the deployment, scaling, and management of containerized applications.
* [Minikube](https://minikube.sigs.k8s.io/docs/start/?arch=%2Fmacos%2Fx86-64%2Fstable%2Fbinary+download) - Local Kubernetes focusing on making it easy to learn and develop for Kubernetes.
* [k9s](https://k9scli.io/) - A terminal based UI to interact with our Kubernetes clusters.
* [sdkman](https://sdkman.io) to experiment with differnt java versions. Using Java 17 for Apache Flink.

## Credentials Setup

Before building and running the project, you need to create two configuration files to store your credentials.

### 1. MQTT Credentials
Create a file at `lib/MQTT/MQTTCredentials.h` with the following content:
```cpp
#pragma once
#define MQTT_SERVER "your-mqtt-broker-address"
#define MQTT_PORT 1883
#define MQTT_USER "your-username"
#define MQTT_PASSWORD "your-password"
```
### 2. Prometheus Credentials
Create a file at lib/Prometheus/Config.h with the following content:
```cpp
#pragma once
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"
#define URL "host.docker.internal"
#define PATH "/api/v1/write"
#define PORT 9090
```
## Infrastructure Setup
The `setup.sh` script will create the containers needed for development & testing.
   * **Note:** Make sure docker is running on your machine.
   ```sh
    cd PlantIQ/
    ./setup.sh
   ```
   Give it a minute and check if the containers are up.
   ```sh
    docker ps -a
   ```
Once everything is up, you can access all services:
* Grafana: http://localhost:3000/
    * initial username/password is *admin*
* Prometheus: http://localhost:9090/
* Apache Flink: http://localhost:8081/

You can now exec into your development container to compile code.
```sh
# Compile src code
docker exec -it dev-container /bin/bash
pio run
```

## Infrastructure Teardown
The `teardown.sh` script will give you the option to teardown specific services.
```sh
Teardown infrastructure/metrics-compose.yml?[y/n]: 
Teardown infrastructure/flink-compose.yml?[y/n]: 
Teardown development environment?[y/n]: 
```

## Build
### Emmbedded Software
PlatformIO will be used to compile the C++ code and upload to the ESP32 board. We primerly use the `pio` CLI command to build, upload, monitor and clean the project. PlatformIO has an extensive list that documents the usage [here](https://docs.platformio.org/en/latest/core/userguide/index.html#usage), but here's a brief summary of the commands we use.

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

## Testing
Apache Flink jobs are used to simulate plant metrics. This establishes a data pipeline to Prometheus therefore providing metrics visualization on Grafana.
### Simulation JAR
There's a sensor data simulator jar that will be utilized to create a flink job. 
```sh
# Create the JAR artifact with the latest changes
docker exec -it dev-container /bin/bash
cd test/sensor-data-generator
mvn clean package
exit
```

### Simulation Setup
You can define however many simulators you want as long as you have the necessary resources.

Ex. Create 2 simulators.
```sh
cd test/
./start-sim.sh
Enter number of data simulators: 2
...
Started sensor-data-sim-03e329 with jobid b417df0baf67c559f0ad696a59728dd9
Started sensor-data-sim-c5127f with jobid 353fdd1bd8ab3f35e743d30f852639f7
Done
```
You can manage & monitor the simulators in the [Apache Flink Dashboard](http://localhost:8081/#/overview). 

Run `./end-sim.sh` to stop all simulators.