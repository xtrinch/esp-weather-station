Import("env")

env.Append(CPPDEFINES=[
  ("SLEEP_SECONDS", 300), # how long the board will sleep between measurements in seconds
  ("SENSOR_DASHBOARD_URL", "sensor-dashboard.trina.si"),
  ("ONBOARD_WIFI_SSID", "ONBOARD-WIFI-SSID"), # onboard AP SSID during configuration
  ("ONBOARD_WIFI_PASSWORD", "ONBOARD-WIFI-PASSWORD") # onboard AP PASSWORD during configuration
])