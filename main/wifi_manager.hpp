#ifndef M5CORE2_TIME_BOX_WIFI_MANAGER_HPP_
#define M5CORE2_TIME_BOX_WIFI_MANAGER_HPP_

class WifiManager {
 public:
  void connect();

  void scanWifi();

  bool isWifiConnected() {
    return connected;
  }
  
  // enable singleton
  static WifiManager& instance() {
    static WifiManager me;
    return me;
  }
  WifiManager(WifiManager const&) = delete; // delete copy construct
  WifiManager(WifiManager&&) = delete; // delete move construct
  WifiManager& operator=(WifiManager const&) = delete; // delete copy assign
  WifiManager& operator=(WifiManager &&) = delete; // delete move assign
 
 private:
  bool connected = false;

  WifiManager() = default;
  ~WifiManager() = default;
};

#endif