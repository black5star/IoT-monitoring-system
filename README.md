# IoT Infrastructure Environmental Monitoring System

## Overview
This project is a localized, professional-grade Internet of Things (IoT) monitoring platform built to safeguard mission-critical hardware environments. It provides continuous surveillance, early anomaly detection, and centralized data visualization for server rooms, network closets, and datacenter racks. 

By constantly tracking key environmental metrics, the system ensures that infrastructure administrators can spot risks before they turn into expensive hardware failures or downtime.

## The Need: Why This System Matters
Critical IT infrastructure faces constant invisible risks:
* **Overheating:** Servers generate massive heat; cooling failures can trigger unexpected emergency thermal shutdowns or permanent component damage.
* **Humidity Fluctuations:** Too much moisture leads to corrosion and short circuits, while too little moisture causes electrostatic discharge risks.
* **Fire Hazards:** Faulty power supplies or electrical overloads can spark fires inside isolated server racks where they are difficult to notice early.
* **Physical Intrusions:** Unauthorized access or physical tampering in remote network closets can compromise physical and digital security.

This system bridges the gap by acting as an automated, 24/7 sentry that provides immediate visibility into the physical health of your infrastructure.

## How It Works
The entire platform operates as a seamless, automated data pipeline split into three clear stages:

1. **Sense (The Edge Node):** A compact hardware device positioned directly inside the server rack uses dedicated sensors to sample physical conditions (temperature, moisture levels, smoke presence, and motion) in real time.
2. **Transport & Store (The Backend Pipeline):** The edge device securely transmits its readings over a stable, wired network link to a centralized message hub. A data collection engine picks up these messages, processes them, and commits them to a specialized time-series database optimized for tracking metrics over time.
3. **Visualize (The Dashboard):** The stored data is pulled into a centralized web dashboard. Administrators can view real-time health gauges, track historical trends over days or weeks, and monitor the safety status of their assets from any web browser.

## Core Tools Used
The system is built using an industry-standard, lightweight software stack designed for maximum reliability and fast performance:

* **Wired ESP32 Microcontroller:** Serves as the brain of the physical edge device, chosen for its industrial reliability and dedicated hardware ethernet connection.
* **Eclipse Mosquitto:** A lightweight message broker that handles high-speed data transmission from the physical sensors to the servers.
* **Telegraf:** An efficient collection agent that captures streaming network data, parses it, and routes it cleanly into storage.
* **InfluxDB:** A high-performance time-series database engine engineered specifically to handle continuous timestamped sensor data.
* **Grafana:** The leading open-source visualization suite used to build intuitive, user-friendly graphs and monitoring dashboards.
* **Docker & Docker Compose:** Containerization tools that bundle the entire server infrastructure so it can be deployed instantly on local server hardware with a single command.

## Project Structure

The repository is cleanly organized into two isolated environments: the physical edge hardware layer and the containerized backend software layer.

```text
├── arduino_esp32/
│   ├── esp32_local_monitoring.ino
│   └── schema.png 
│
└── backend/
    ├── docker-compose.yml
    ├── Makefile
    │
    ├── mosquitto/
    │   └── mosquitto.conf
    │
    └── telegraf/
        └── telegraf.conf
```

### Directory Deep-Dive

#### 📁 `arduino_esp32/` (Edge Firmware)
This directory acts as the firmware root for the physical deployment unit.
* **`esp32_local_monitoring.ino`**: The microcontroller source code. It handles network handshakes, reads physical voltages from the sensors, packages metrics into uniform data lines, and publishes them across the network.
* **`schema.png`**: The blueprint mapping how the physical sensors (temperature, smoke, motion) route into the pins of the microcontroller board.

#### 📁 `backend/` (Infrastructure Orchestration)
This directory acts as the control room for the server environment.
* **`docker-compose.yml`**: The master configuration that builds, links, and runs all four background services seamlessly inside isolated environments.
* **`Makefile`**: Short, automated terminal aliases that simplify complex container commands into quick, daily-use functions like starting, stopping, or viewing diagnostic outputs.

#### 📁 `backend/mosquitto/` (Message Broker Hub)
* **`mosquitto.conf`**: Sets the behavior rules for the network broker. It ensures the ingestion endpoint is open on the correct port and configured to cleanly receive continuous payloads from the edge node.

#### 📁 `backend/telegraf/` (The Data Pipeline Translator)
* **`telegraf.conf`**: The mapping engine. It instructs the service to subscribe to the incoming sensor network topic, grab the raw telemetry lines, and continuously push them cleanly into the time-series database.


