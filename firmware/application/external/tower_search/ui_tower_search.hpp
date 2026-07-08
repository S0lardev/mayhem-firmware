#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "string_format.hpp"

//log
#include "log_file.hpp"

namespace ui
{
    // TODO: replace with a proper class in ui_receiver.hpp
    struct TowerSearchRecentEntry {
    using Key = uint64_t;
    static constexpr Key invalid_key = 0x0fffffff;
    uint8_t rat = LTE;  // 0=NR, 1=LTE, 2=UMTS
    uint16_t bits = 0;
    uint16_t age = 0;  // updated on each seconds, show how long the signal was last seen
    uint64_t data = 0;
    TowerSearchRecentEntry() {}
    TowerSearchRecentEntry(
        uint8_t sensorType,
        uint64_t data = 0,
        uint16_t bits = 0);
        
    void inc_age(int delta) {
        if (UINT16_MAX - delta > age) age += delta;
    }
    void reset_age() {
        age = 0;
    }

    std::string to_csv();
};

    class nrTowerLog {
   public:
    Optional<File::Error> append(const std::filesystem::path& filename) {
        return log_file.append(filename);
    }

    void log_data(TowerSearchRecentEntry& data);
    void write_header() {
        log_file.write_entry(";Type; Bits; Data;");
    }

   private:

    LogFile log_file{};
};
    class TowerSearchView : public View                                // App class declaration
    {
    public:
        TowerSearchView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "Tower"; }; // App title
        void focus() override;
    void refresh();

    private:
        void start_scan_thread();
        void stop_scan_thread();
        void on_data(const SubGhzDDataMessage* data);

        void update();                                            // Function declaration
        MessageHandlerRegistration message_handler_update{        // Example, not required: MessageHandlerRegistration class
            Message::ID::DisplayFrameSync,                        // relays messages to your app code from baseband. Every time you 
            [this](const Message *const) {                        // get a  DisplayFrameSync message the update() function will
                this->update();                                   // be triggered.  
            }};

        OptionsField options_frequency{
        {UI_POS_X(0), UI_POS_Y(0)},
        7,
        {}};

    ui::NavigationView& nav_;

    ui::RFAmpField field_rf_amp{
        {UI_POS_X(8), UI_POS_Y(0)}};

    ui::LNAGainField field_lna{
        {UI_POS_X(10), UI_POS_Y(0)}};

    ui::VGAGainField field_vga{
        {UI_POS_X(13), UI_POS_Y(0)}};

    ui::RSSI rssi{
        {UI_POS_X(16), UI_POS_Y(0), UI_POS_WIDTH_REMAINING(22), 4}};

    ui::Channel channel{
        {UI_POS_X(16), UI_POS_Y(0) + 5, UI_POS_WIDTH_REMAINING(22), 4}};

    // ui::Audio audio{
    //     {UI_POS_X(16), UI_POS_Y(0) + 10, UI_POS_WIDTH_REMAINING(22), 4}};

    ui::AudioVolumeField field_volume{
        {UI_POS_WIDTH_REMAINING(2), UI_POS_Y(0)}};

        
    };
    Button start_scan{
        {0, 16, 7 * 8, 32},
        "Start"};

    Checkbox check_nr{
        {10 * 8, 18},
        3,
        "5g",
        true};

    Checkbox check_lte{
        {10 * 8, 18 + 8},
        3,
        "4g",
        true};

    Checkbox check_umts{
        {10 * 8, 18 + 16},
        3,
        "3g",
        true};

    
} 