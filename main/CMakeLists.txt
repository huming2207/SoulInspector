idf_component_register(
        SRCS
            "si_main.cpp"
            "driver/uart_data_broker.cpp" "driver/uart_data_broker.hpp"
            "storage/config_manager.cpp" "storage/config_manager.hpp"
            "misc/eternal.hpp"

        INCLUDE_DIRS
            "driver" "storage" "protocol" "misc"
)
