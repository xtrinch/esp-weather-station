Import("env")

env.Append(CPPDEFINES=[
  ("WIFI_SSID", "ssid"),
	("WIFI_PASSWORD", "pass"),
  ("SENSOR_DASHBOARD_URL", "iotfreezer.com"),
  ("ACCESS_TOKEN", "token")
])