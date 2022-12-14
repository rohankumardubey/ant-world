#ifndef YDS_WINDOW_SYSTEM_H
#define YDS_WINDOW_SYSTEM_H

#include "yds_dynamic_array.h"
#include "yds_window.h"
#include "yds_monitor.h"

class ysInputSystem;

class ysWindowSystem : public ysWindowSystemObject {
protected:
    // Singelton interface (necessary for WndProc which is a global function)
    static ysWindowSystem *Get();
    static ysWindowSystem *g_instance;

    ysWindowSystem();
    ysWindowSystem(Platform platform);
    virtual ~ysWindowSystem();

public:
    static ysError CreateWindowSystem(ysWindowSystem **newSystem, Platform platform);
    static ysError DestroyWindowSystem(ysWindowSystem *&system);

    // Window Interface
    virtual ysError NewWindow(ysWindow **window) = 0;

    void CloseWindow(ysWindow *window);
    void DeleteWindow(ysWindow *window);

    // Monitor Interface
    virtual ysMonitor *NewMonitor() = 0;
    virtual void SurveyMonitors();

    void ClearMonitors();
    ysMonitor *GetPrimaryMonitor();
    ysMonitor *GetMonitor(int index);
    int GetNumMonitors();

    // Message Interface
    virtual void ProcessMessages() = 0;

    void CloseAllWindows();
    void DeleteInactiveWindows();
    void DeleteAllWindows();

    ysError AssignInputSystem(ysInputSystem *system);
    ysInputSystem *GetInputSystem() { return m_inputSystem; }

    virtual void ConnectInstance(void *genericInstanceConnection) { (void)genericInstanceConnection; }

protected:
    ysDynamicArray<ysMonitor, 4> m_monitorArray;
    ysDynamicArray<ysWindow, 4> m_windowArray;

    ysInputSystem *m_inputSystem;
};

#endif /* YDS_WINDOW_SYSTEM_H */
