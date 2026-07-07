#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "string_format.hpp"

namespace ui
{
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
        void nr_scan_thread();
        void lte_scan_thread();
        void umts_scan_thread();
        
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
} 