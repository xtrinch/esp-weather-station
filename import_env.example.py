Import("env")

env.Append(CPPDEFINES=[
  ("WIFI_SSID", "ssid"),
	("WIFI_PASSWORD", "pass"),
  ("ACCESS_TOKEN", "token"),
  ("PRECONFIGURED"),
  ("OPENWEATHERMAP_LOCATION", "Maribor%2CSlovenia"),
  ("OPENWEATHERMAP_APPID", "yourAppId"),
  ("USER_SETUP_LOADED", "1"),
  ("ILI9341_DRIVER", "1"),
  ("TFT_MISO", "19"),
  ("TFT_MOSI", "23"),
  ("TFT_SCLK", "18"),
  ("TFT_CS", "5"),
  ("TFT_RST", "17"),
  ("TFT_DC", "16"),
  ("SPI_FREQUENCY", "27000000"),
  ("SPI_READ_FREQUENCY", "20000000"),
  ("SPI_TOUCH_FREQUENCY", "2500000"),
  ("SMOOTH_FONT", "1"),
  ("DISPLAY_REFRESH_RATE", 60), # seconds
])
