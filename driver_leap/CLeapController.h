#pragma once

class CControllerButton;

class CLeapController : public vr::ITrackedDeviceServerDriver
{
    static vr::IVRServerDriverHost *ms_driverHost;
    static double ms_headPosition[3];
    static vr::HmdQuaternion_t ms_headRotation;
    vr::DriverPose_t m_pose;

    glm::quat m_gripOffset;

    CLeapController(const CLeapController &that) = delete;
    CLeapController& operator=(const CLeapController &that) = delete;

    void ResetControls();
    void UpdateInput();
    void UpdateTransformation(const Leap::Frame &f_frame);

    // vr::ITrackedDeviceServerDriver
    void Deactivate();
    void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {}
    void EnterStandby() {}
    void* GetComponent(const char* pchComponentNameAndVersion);
    vr::DriverPose_t GetPose();
public:
    enum ControllerHand : unsigned char
    {
        CH_Left = 0U,
        CH_Right,

        CH_Count
    };
    enum GameProfile : size_t
    {
        GP_Default = 0U,
        GP_VRChat
    };

    CLeapController();
    virtual ~CLeapController();

    bool GetEnabled() const;
    void SetEnabled(bool f_state);

    inline unsigned char GetHand() const { return m_controllerHand; }
    inline const std::string& GetSerialNumber() const { return m_serialNumber; }

    void SetGameProfile(GameProfile f_profile);

    void SwitchSpecialMode();

    void Update(const Leap::Frame& f_frame);

    static void SetInterfaces(vr::IVRServerDriverHost *f_host, vr::IVRDriverInput *f_input, vr::CVRPropertyHelpers *f_helpers);
    static void UpdateHMDCoordinates();
protected:
    static vr::IVRDriverInput *ms_driverInput;
    static vr::CVRPropertyHelpers *ms_propertyHelpers;
    vr::VRInputComponentHandle_t m_haptic;
    vr::PropertyContainerHandle_t m_propertyContainer;
    uint32_t m_trackedDevice;

    std::vector<CControllerButton*> m_buttons;
    unsigned char m_controllerHand;
    GameProfile m_gameProfile;
    std::string m_serialNumber;
    bool m_specialMode;

    virtual bool MixHandState(bool f_state);
    virtual void UpdateGestures(const Leap::Frame &f_frame) = 0;
    virtual void UpdateInputInternal() {}

    // vr::ITrackedDeviceServerDriver
    virtual vr::EVRInitError Activate(uint32_t unObjectId) = 0;
};