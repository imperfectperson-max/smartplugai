# Data Analytics Use Case Diagram

This diagram illustrates the analytics and reporting capabilities of the Smart Plug AI platform.

## Overview

This use case diagram shows real-time monitoring, historical analysis, cost tracking, reporting, alerting, and advanced analytics features.

## Mermaid Diagram

<!-- Note: This diagram contains comprehensive data analytics use cases with real-time data flow and anomaly detection features -->

```mermaid
graph TB
    subgraph "Analytics & Reporting System"
        %% Actors
        Homeowner[👤 Homeowner]
        FacilityManager[🏢 Facility Manager]
        EnergyAnalyst[📊 Energy Analyst]
        Dashboard[💻 Web Dashboard]
        Mobile[📱 Mobile App]
        Database[🗄️ Database System]


        %% Real-time Monitoring
        UC301["UC-301: View Live Power Stream"]
        UC302["UC-302: Monitor Current Consumption"]
        UC303["UC-303: Track Voltage Stability"]
        UC304["UC-304: Watch Energy Usage Patterns"]


        %% Historical Analysis
        UC305["UC-305: Analyze Daily Usage"]
        UC306["UC-306: Review Weekly Trends"]
        UC307["UC-307: Compare Monthly Data"]
        UC308["UC-308: View Year-over-Year"]


        %% Cost & Efficiency
        UC309["UC-309: Calculate Energy Costs"]
        UC310["UC-310: Estimate Bill Projections"]
        UC311["UC-311: Identify Waste Patterns"]
        UC312["UC-312: Suggest Efficiency Improvements"]


        %% Reporting
        UC313["UC-313: Generate Usage Reports"]
        UC314["UC-314: Export Data (CSV/PDF)"]
        UC315["UC-315: Schedule Automated Reports"]
        UC316["UC-316: Share Reports Securely"]


        %% Alerting
        UC317["UC-317: Set Usage Thresholds"]
        UC318["UC-318: Configure Alert Rules"]
        UC319["UC-319: Receive Usage Alerts"]
        UC320["UC-320: Review Alert History"]


        %% Advanced Analytics
        UC321["UC-321: Run Predictive Analysis"]
        UC322["UC-322: Generate Load Profiles"]
        UC323["UC-323: Perform Anomaly Detection"]
        UC324["UC-324: Create Custom Dashboards"]
    end


    %% Homeowner Relationships
    Homeowner --> UC301
    Homeowner --> UC302
    Homeowner --> UC305
    Homeowner --> UC309
    Homeowner --> UC310
    Homeowner --> UC311
    Homeowner --> UC317
    Homeowner --> UC319


    %% Facility Manager Relationships
    FacilityManager --> UC301
    FacilityManager --> UC306
    FacilityManager --> UC307
    FacilityManager --> UC309
    FacilityManager --> UC313
    FacilityManager --> UC314
    FacilityManager --> UC315
    FacilityManager --> UC321
    FacilityManager --> UC322
    FacilityManager --> UC324


    %% Energy Analyst Relationships
    EnergyAnalyst --> UC307
    EnergyAnalyst --> UC308
    EnergyAnalyst --> UC321
    EnergyAnalyst --> UC322
    EnergyAnalyst --> UC323
    EnergyAnalyst --> UC324


    %% Web Dashboard Relationships
    Dashboard -->|"displays"| UC301
    Dashboard -->|"charts"| UC305
    Dashboard -->|"generates"| UC313
    Dashboard -->|"exports"| UC314
    Dashboard -->|"customizes"| UC324


    %% Mobile App Relationships
    Mobile -->|"streams"| UC301
    Mobile -->|"shows"| UC302
    Mobile -.->|"notifies"| UC319


    %% Database Relationships
    Database -.->|"stores historical data"| UC305
    Database -.->|"queries for reports"| UC313
    Database -.->|"provides training data"| UC321


    %% Include Relationships
    UC301 -.->|"<<include>>"| UC302
    UC305 -.->|"<<include>>"| UC304
    UC313 -.->|"<<include>>"| UC309
    UC321 -.->|"<<include>>"| UC305


    %% Extend Relationships
    UC319 -.->|"<<extend>>"| UC301
    UC323 -.->|"<<extend>>"| UC304
    UC324 -.->|"<<extend>>"| UC301
```

## Real-time Data Flow

1. Device → MQTT over TLS 1.3
2. Backend → WebSocket over TLS
3. Client → Encrypted display
4. Database → Encrypted at rest
5. Analytics → Field-level encryption

## Anomaly Detection

The system monitors for:
- Unusual power spikes
- Unexpected consumption patterns
- Tamper detection via MAX6316
- Firmware integrity violations
- Network security anomalies

## Security Features

- End-to-end encryption for all data
- Field-level encryption for sensitive information
- Real-time anomaly detection
- Secure data aggregation
- Encrypted exports
- Access control for reports
