#ifndef __UI_TOWER_H__
#define __UI_TOWER_H__

#include "app_settings.hpp"
#include "log_file.hpp"
#include "recent_entries.hpp"
#include "ui_freq_field.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "ui_widget.hpp"
#include <array>
#include <cstdint>

namespace ui::external_app::tower_search {

enum class RadioAccessTechnology : uint8_t {
    umts,
    lte,
    nr,
};

struct TowerSearchResult {
    RadioAccessTechnology rat{RadioAccessTechnology::umts};
    rf::Frequency frequency{0};
    uint32_t channel{0};
    uint32_t cell_id{0};
    int16_t rssi{0};
};

struct TowerSearchRecentEntry {
    using Key = uint64_t;
    static constexpr Key invalid_key = 0x0fffffff;

    RadioAccessTechnology rat{RadioAccessTechnology::umts};
    uint16_t bits{0};
    uint16_t age{0};
    uint64_t data{0};

    TowerSearchRecentEntry() = default;
    TowerSearchRecentEntry(
        RadioAccessTechnology rat,
        uint64_t data = 0,
        uint16_t bits = 0);

    void inc_age(uint16_t delta);
    void reset_age();
    std::string to_csv() const;
};

class TowerLogger {
   public:
    Optional<File::Error> append(const std::filesystem::path& filename);
    void log_data(const TowerSearchRecentEntry& data);
    void write_header();

   private:
    LogFile log_file{};
};

using TowerSearchRecentEntries = RecentEntries<TowerSearchRecentEntry>;
using TowerSearchRecentEntriesView = RecentEntriesView<TowerSearchRecentEntries>;

class TowerSearchView : public View {
   public:
    explicit TowerSearchView(NavigationView& nav);
    std::string title() const override { return "Tower"; }
    void focus() override;
    void refresh();

    void configure_scan(RadioAccessTechnology rat, bool enabled);
    void start_scan();
    void stop_scan();
    void scan_next_frequency();
    bool detect_sync_signal(const std::array<uint8_t, 256>& samples) const;
    bool decode_broadcast_channel(
        const std::array<uint8_t, 256>& samples,
        TowerSearchResult& result) const;
    void handle_scan_result(const TowerSearchResult& result);

   private:
    uint32_t progress{0};
    bool logging_enabled{false};
    bool scanning{false};
    bool umts_enabled{true};
    bool lte_enabled{false};
    bool nr_enabled{false};
    app_settings::SettingsManager settings_{
        "rx_tower_search",
        app_settings::Mode::RX,
        {{"log"sv, &logging_enabled}}};
    TowerLogger logger{};
    NavigationView& nav_;

    void update();
    void start_scan_thread();
    void stop_scan_thread();
    void on_data(const TowerSearchRecentEntry* data);
};

}  // namespace ui::external_app::tower_search

#endif /* __UI_TOWER_H__ */
