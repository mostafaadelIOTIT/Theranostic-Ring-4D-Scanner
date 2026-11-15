```markdown
# 🦾 Dynamic Theranostic Ring Scanner – 4D Diabetic Limb Diagnostics & Phototherapy


**Lead Architect:** Mostafa Adel Muhammed (B.Sc. in Information Technology, GPA: 3.09 | IT Systems & Cybersecurity Specialist)  
**LinkedIn:** [mostafa-adel-iotit](https://www.linkedin.com/in/mostafa-adel-iotit/)


## 📌 Executive Summary
The **Dynamic Theranostic Ring Scanner** is an automated cyber-physical medical prototype designed to prevent diabetic foot amputations. By gliding a motorized 360-degree ring around the affected extremity, the system merges contactless **Micro-Thermal & Near-Infrared (NIR) Spectral Mapping** with **Hyperspectral/Photoacoustic reconstruction** to detect deep neuropathic inflammation, ischemia, and fungal biofilms before ulcers break the skin—immediately delivering targeted **Low-Level Laser Therapy (LLLT)** and **UV-C sterilization**.


## 🚀 Live Interactive Simulations (Click Below to Test in Browser)
* 🔗 **[Launch 4D Clinical PACS & Security Dashboard (Streamlit Cloud)](https://theranostic-ring-4d-scanner-evybtjw9hkrybfiq3av8lt.streamlit.app/)**
* 🔗 **[Run Real-Time ESP32 & Stepper Gantry Simulation (Wokwi)](https://wokwi.com/projects/480753689106439169)**


## 🏗️ System Architecture & Cybersecurity
1. **Embedded Edge Control (`firmware_esp32.ino`):** C++ firmware running on ESP32 driving an A4988 stepper gantry across 360° limb sectors, reading sub-dermal thermal gradients and NIR fungal indices, and automatically locking onto pathology coordinates to trigger LLLT (660nm/850nm) and UV-C (275nm) emitters.
2. **Zero-Trust Medical Cryptography:** Utilizes hardware-accelerated `mbedtls` **SHA-256 hashing** at the microcontroller edge to sign every diagnostic packet, ensuring GDPR/HIPAA compliance and preventing Man-in-the-Middle (MitM) tampering.
3. **Clinical PACS & Odoo ERP Integration (`app.py`):** Python/Streamlit interface visualizing 360° limb sector telemetry and verifying cryptographic signatures for hospital ERP archiving.
```
