#pragma once
#include <dwmapi.h>

struct Monitor
{
public:
    Monitor(nullptr_t) {}
    Monitor(HMONITOR monitor, std::wstring const& name)
    {
        m_monitor = monitor;
        m_name = name;
    }

    HMONITOR Hmonitor() const noexcept { return m_monitor; }
    std::wstring Name() const noexcept { return m_name; }

private:
    HMONITOR m_monitor;
    std::wstring m_name;
};

BOOL WINAPI MonitorEnumCallback(HMONITOR monitor,
  HDC hdc,
  LPRECT lprc,
  LPARAM data) {

  MONITORINFOEX info_ex;
  info_ex.cbSize = sizeof(MONITORINFOEX);

  GetMonitorInfo(monitor, &info_ex);

  // https://jira.agoralab.co/browse/CSD-26297
  // mirror mode or non-active do not return
  if (info_ex.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER)
    return true;

  auto monitors = ((std::vector<Monitor>*)data);

//   WindowEnumer::MONITOR_INFO info;
//   info.index = monitors->size();
//   info.rc = info_ex.rcMonitor;
//   info.name = info_ex.szDevice;
//   info.is_primary = info_ex.dwFlags & MONITORINFOF_PRIMARY;


  monitors->emplace_back(Monitor(monitor, info_ex.szDevice));

  return true;
}

std::vector<Monitor> EnumerateMonitors()
{
  std::vector<Monitor> monitors;

  ::EnumDisplayMonitors(NULL, NULL, MonitorEnumCallback, (LPARAM)&monitors);

  return monitors;
}