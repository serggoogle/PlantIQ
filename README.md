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
For local development, it's recommened to have the following tools:
* Java 17 JDK
    * I recommend using [sdkman](https://sdkman.io) to experiment with differnt java versions.
* [Docker](https://docs.docker.com/get-started/) - Containerization technology for building and containerizing your applications
* [Kubernetes](https://kubernetes.io/docs/tasks/tools/install-kubectl-macos/) - open-source platform for automating the deployment, scaling, and management of containerized applications.
* [Minikube](https://minikube.sigs.k8s.io/docs/start/?arch=%2Fmacos%2Fx86-64%2Fstable%2Fbinary+download) - Local Kubernetes focusing on making it easy to learn and develop for Kubernetes.
* [k9s](https://k9scli.io/) - A terminal based UI to interact with our Kubernetes clusters.
* [PlatformIO](https://platformio.org/?utm_source=platformio&utm_medium=piohome) - For Embedded Software Development

### Build
> TODO
