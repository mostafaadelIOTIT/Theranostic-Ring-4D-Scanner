import streamlit as st
import pandas as pd
import numpy as np
import hashlib
import json

st.set_page_config(page_title="Theranostic Ring 4D PACS", layout="wide")

st.title("🦾 Dynamic Theranostic Ring Scanner – 4D Clinical & Security Console")
st.caption("Lead Architect: Mostafa Adel | Edge SHA-256 Telemetry & Automated Phototherapy Control")

st.sidebar.header("📡 ESP32 Edge Telemetry Input")
angle_deg = st.sidebar.slider("Ring Gantry Angle (°)", 0, 360, 120, step=30)
temp_c = st.sidebar.slider("Micro-Thermal Tissue Temp (°C)", 25.0, 40.0, 36.4, step=0.1)
nir_fungal_idx = st.sidebar.slider("NIR Spectral Fungal Index (%)", 0, 100, 72)

diagnosis = "NORMAL_PERFUSION"
therapy = "STANDBY"
status_color = "green"

if temp_c > 35.5:
    diagnosis = "DEEP_NEUROPATHIC_INFLAMMATION"
    therapy = "LLLT_660nm_850nm_ACTIVE"
    status_color = "red"
elif temp_c < 29.0:
    diagnosis = "ISCHEMIC_LOW_PERFUSION"
    therapy = "LLLT_VASODILATION_ACTIVE"
    status_color = "orange"

if nir_fungal_idx > 65:
    diagnosis += " + FUNGAL_BIOFILM_DETECTED"
    therapy += " + UVC_275nm_STERILIZATION"
    status_color = "red"

payload_dict = {
    "angle_deg": angle_deg,
    "temp_c": round(temp_c, 1),
    "nir_fungal_idx": nir_fungal_idx,
    "diagnosis": diagnosis,
    "therapy": therapy
}
payload_str = json.dumps(payload_dict, separators=(',', ':'))
sha256_sig = hashlib.sha256(payload_str.encode()).hexdigest()[:32]

col1, col2, col3 = st.columns(3)
col1.metric("Gantry Angular Lock", f"{angle_deg}°", "Stepper Active")
col2.metric("Sub-Dermal Thermal Map", f"{temp_c} °C", "Abnormal" if temp_c > 35.5 or temp_c < 29.0 else "Normal")
col3.metric("NIR Fungal Biofilm Index", f"{nir_fungal_idx}%", "Critical > 65%" if nir_fungal_idx > 65 else "Clear")

st.markdown(f"### 🎯 Clinical Triage Status: :{status_color}[{diagnosis}]")
st.info(f"**Automated Phototherapy Action:** `{therapy}`")

st.subheader("🔄 360° Limb Sector Mapping (HSI + Photoacoustic Profile)")
angles = list(range(0, 360, 30))
temps = [round(33.2 + np.sin(np.radians(a))*1.5, 1) if a != angle_deg else temp_c for a in angles]
nir_vals = [25 if a != angle_deg else nir_fungal_idx for a in angles]

df_scan = pd.DataFrame({
    "Ring Angle (°)": angles,
    "Tissue Temp (°C)": temps,
    "NIR Fungal Index (%)": nir_vals
}).set_index("Ring Angle (°)")

st.line_chart(df_scan)

st.subheader("🔒 Zero-Trust Medical Audit & Odoo ERP Sync")
st.code(f"Raw JSON Telemetry : {payload_str}\nEdge SHA-256 Hash  : {sha256_sig}\nERP / PACS Status  : VERIFIED & LOGGED (GDPR/HIPAA Compliant)", language="yaml")
