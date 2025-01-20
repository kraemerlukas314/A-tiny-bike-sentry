# A Tiny Bike Sentry

[🇩🇪 Deutsch](#deutsch) | [🇬🇧 English](#english)

---

## 🇩🇪 Deutsch

### Überblick
A Tiny Bike Sentry ist ein elektronisches Fahrradschloss, das bei Erschütterungen einen optischen und akustischen Alarm auslöst, um Diebstähle zu verhindern. Es basiert auf einem stromsparenden ATtiny85-Mikrocontroller und verwendet eine LED, einen Buzzer, einen Taster und einen Erschütterungssensor.

### Funktionen
- **Erschütterungserkennung**: Erkennt Bewegung mit einem Piezoelement und löst einen Alarm aus.
- **Akustische & optische Warnsignale**: Alarmiert mit einem Buzzer und einer LED.
- **Energiesparmodus**: Minimiert Stromverbrauch durch effiziente Software- und Hardwarearchitektur.
- **Manuelle Aktivierung**: Aktivierung und Deaktivierung per Knopfdruck.

### Hardware
- Mikrocontroller: **ATtiny85** (niedriger Stromverbrauch, kompakte Größe)
- Sensoren: **Piezoelement** zur Erschütterungserkennung
- Aktoren: **LED und Buzzer** zur Alarmierung
- Stromversorgung: **Akku- oder Batteriebetrieb** (optimiert für lange Laufzeit)

### Software
Die Software wurde in **C++** geschrieben und basiert auf einer **zustandsbasierten Programmierung**.
Wichtige Dateien:
- `main.ino`: Hauptprogramm mit der Zustandsmaschine
- `Gpio.h / Gpio.cpp`: Steuerung der Hardware-Pins
- `Animation.h / Animation.cpp`: Verwaltung der LED- und Buzzer-Signale
- `Timing.h / Timing.cpp`: Optimierung der Zeitsteuerung
- `defines.h`: Konfigurationsdatei mit Schwellenwerten & Pin-Belegung

### Zustandsdiagramm
Das System arbeitet mit einer **Finite State Machine**:
- **Deep Sleep**: Gerät inaktiv, wartet auf Aktivierung
- **Sentry**: Gerät ist scharfgestellt und prüft auf Erschütterungen
- **Attention**: Vorwarnung nach erster Erschütterung
- **Alarm**: Vollständiger Alarmmodus
- **Cooldown**: Nach einer bestimmten Zeit ohne weitere Bewegung geht das System zurück in den Sentry-Modus

(Siehe `State Machine Graph.pdf` für visuelle Darstellung)

### Installation & Nutzung
#### Voraussetzungen
- **Arduino IDE oder CLI**
- **ATtiny85 Board-Treiber** (via Board Manager installieren)
- **Arduino as ISP Programmer**

#### Kompilieren & Hochladen
1. ATtiny85 als Board in der Arduino IDE auswählen
2. `Programmer: Arduino as ISP` einstellen
3. Sketch mit `Sketch -> Hochladen über Programmer` übertragen
4. Gerät mit Strom versorgen und testen

### Dokumentation
Detaillierte Informationen zur Implementierung sind in folgenden Dateien enthalten:
- `Benutzerdokumentation.pdf`: Anleitung für Endnutzer
- `Entwicklerdokumentation.pdf`: Technische Details & Implementierung
- `Doxygen refman.pdf`: Generierte Code-Dokumentation
- `Präsentation.pdf`: Überblick und Projektvorstellung

### Weiterentwicklung
- Implementierung eines **Bluetooth-Moduls** zur Fernsteuerung
- **Akkubetrieb mit Ladeschaltung** für mehr Flexibilität
- **Erweiterte Sicherheitsmechanismen** (z. B. Code-Eingabe oder Smartphone-Authentifizierung)

### Lizenz
Dieses Projekt steht unter der **MIT-Lizenz** – freie Nutzung und Modifikation sind erlaubt.

---

## 🇬🇧 English

### Overview
A Tiny Bike Sentry is an electronic bike lock that triggers an optical and acoustic alarm when vibrations are detected, helping to prevent theft. It is based on a low-power ATtiny85 microcontroller and uses an LED, a buzzer, a button, and a vibration sensor.

### Features
- **Vibration detection**: Detects movement with a piezo element and triggers an alarm.
- **Acoustic & optical warning signals**: Alerts with a buzzer and LED.
- **Energy-saving mode**: Minimizes power consumption through an efficient software and hardware architecture.
- **Manual activation**: Activate and deactivate via a button press.

### Hardware
- Microcontroller: **ATtiny85** (low power consumption, compact size)
- Sensors: **Piezo element** for vibration detection
- Actuators: **LED and buzzer** for alarming
- Power supply: **Battery or rechargeable operation** (optimized for long battery life)

### Software
The software is written in **C++** and is based on a **state-based programming** approach.
Key files:
- `main.ino`: Main program with the state machine
- `Gpio.h / Gpio.cpp`: Controls hardware pins
- `Animation.h / Animation.cpp`: Manages LED and buzzer signals
- `Timing.h / Timing.cpp`: Optimizes time control
- `defines.h`: Configuration file with thresholds & pin assignments

### State Diagram
The system operates with a **Finite State Machine**:
- **Deep Sleep**: Device is inactive, waiting for activation
- **Sentry**: Device is armed and checking for vibrations
- **Attention**: Pre-warning after initial vibration detection
- **Alarm**: Full alarm mode
- **Cooldown**: After a certain time without further movement, the system returns to the Sentry mode

(See `State Machine Graph.pdf` for a visual representation)

### Installation & Usage
#### Requirements
- **Arduino IDE or CLI**
- **ATtiny85 board driver** (install via board manager)
- **Arduino as ISP programmer**

#### Compiling & Uploading
1. Select ATtiny85 as board in Arduino IDE
2. Set `Programmer: Arduino as ISP`
3. Upload the sketch using `Sketch -> Upload Using Programmer`
4. Power the device and test

### Documentation
Detailed implementation information can be found in the following files:
- `Benutzerdokumentation.pdf`: User manual
- `Entwicklerdokumentation.pdf`: Technical details & implementation
- `Doxygen refman.pdf`: Generated code documentation
- `Präsentation.pdf`: Overview and project presentation

### Future Improvements
- **Bluetooth module** for remote control
- **Battery-powered operation with charging circuit** for more flexibility
- **Advanced security mechanisms** (e.g., code entry or smartphone authentication)

### License
This project is released under the **MIT License** – free to use and modify.

---

**Project by Lukas Krämer | Winter Semester 2024**  
[GitHub Repository](https://github.com/kraemerlukas314/A-tiny-bike-sentry)
