#ifndef __UI_TOWER_H__
#define __UI_TOWER_H__

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "string_format.hpp"
#include "app_settings.hpp"
#include "recent_entries.hpp"
#include "ui_freq_field.hpp"
#include "utility.hpp"
//log
#include "log_file.hpp"

using namespace ui;

namespace ui::external_app::tower_search {
    #define PROGRESS_MAX 100
    #define OOK_SAMPLERATE_DEFAULT 2280000U // Set the default Sample Rate
    #define RECEIVE_FREQUENCY_DEFAULT 433920000U   // Sets the default receive frequency (27 MHz).
    #define WAVEFORM_BUFFER_SIZE 550
   
   
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


    class TowerLogger {
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

using TowerSearchRecentEntries = RecentEntries<TowerSearchRecentEntry>;
using TowerSearchRecentEntriesView = RecentEntriesView<TowerSearchRecentEntries>;
    class TowerSearchView : public View                                // App class declaration
    {
    public:
        TowerSearchView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "Tower"; }; // App title
        void focus() override;
    void refresh();

    private:
        
        unit32_t progress = 0;
        bool logging = false;
        bool scanning = false;
        bool umts_enabled = true;
        app_settings::SettingsManager settings_{"rx_tower_search", app_settings::Mode::RX, {{"log"sv, &logging},}};  // App settings manager
        TowerLogger logging{};
        void start_scan_thread();
        void stop_scan_thread();
        void on_data(const SubGhzDDataMessage* data);

        void update();                                            // update the view

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
        RecentEntries<TowerSearchRecentEntry> recent{};

         RFAmpField field_rf_amp{
        {13 * 8, UI_POS_Y(0)}};
        LNAGainField field_lna{
        {15 * 8, UI_POS_Y(0)}};
        VGAGainField field_vga{
        {18 * 8, UI_POS_Y(0)}};
        RSSI rssi{
        {21 * 8, 0, UI_POS_WIDTH_REMAINING(24), 4}};
        Channel channel{
        {21 * 8, 5, UI_POS_WIDTH_REMAINING(24), 4},
        };
        
        RxFrequencyField field_frequency{
        {UI_POS_X(0), UI_POS_Y(0)},
        nav_};

        RecentEntriesColumns columns{{
        {"Type", 0},
        {"Bits", 4},
        {"Age", 3},
    }};
    TowerSearchRecentEntriesView recent_entries_view{columns, recent};
        
    };
    Button start_scan{
        {0, 16, 7 * 8, 32},
        "Start"};

    Checkbox check_nr{
        {10 * 8, 18},
        3,
        "5g",
        false};

    Checkbox check_lte{
        {10 * 8, 18 + 8},
        3,
        "4g",
        false};

    Checkbox check_umts{
        {10 * 8, 18 + 16},
        3,
        "3g",
        true};

    
} 

#endif /*__UI_TOWER_H__*/