# Architecture

Coming soon — system architecture and diagrams for Smart Plug AI.

This document will describe the full system architecture including:

- Device layer: ESP32 devices, sensors (INA219/SCT-013), relays
- Communication: MQTT topics, message formats, QoS
- Application layer: FastAPI services, auth, caching
- Data layer: InfluxDB (time-series), PostgreSQL (metadata), Snowflake (OLAP)
- ML/AI: model training pipelines, where models are hosted (edge vs cloud)
- Deployment: docker-compose, Kubernetes suggestions, CI/CD

Add diagrams (ASCII or images) and example data flows for telemetry ingestion and user-facing APIs.
