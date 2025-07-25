# Plant Monitoring Service 🍃🌿☘️
> A modernized monitoring system that provides insight to a plants lifecylce 🩺

## Table of Contents
<!-- "## " is H2. Use this section for easy navigation in large READMEs -->
- [Overview](#overview)
- [Installation](#installation)
- [Buid](#build)
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
   git clone https://github.com/yourusername/PlantMonitoringService.git
   ```

## Infrastructure Setup
   ```sh
    cd PlantMonitoringService/infra
    docker compose up -d
   ```
   Give it a minute and check if the containers are up.
   ```sh
    docker ps -a
   ```
   * **Note:** Make sure docker is running on your machine or i'll throw some error.

Once everything is up, you can access both services:
* Grafana: http://localhost:3000/
    * initial username/password is *admin*
* Prometheus: http://localhost:9090/

TODO: Add steps to import the *Plant-Metrics-Template.json* dashboard located under infra/grafana/

## Build

PlatformIO will be used to compile the C++ code and upload to the Arduino board. We primerly use the `pio` CLI command to build, upload, monitor and clean the project. PlatformIO has an extensive list that documents the usage [here](https://docs.platformio.org/en/latest/core/userguide/index.html#usage), but here's a brief summary of the commands we use and what they do.

`pio run`
- Compiles (builds) your project and generates the firmware binary for your board.

`pio run -t upload`
- Builds your project (if needed) and uploads (flashes) the compiled firmware to your connected microcontroller board.

`pio run -t monitor`
- Builds your project (if needed) and opens the PlatformIO Serial Monitor to view serial output from your device.

`pio run -t clean`
- Removes all build artifacts (compiled files, firmware binaries, etc.) from your project’s .pio build directory, allowing you to do a fresh rebuild next time you compile.
## Test
TODO