idf_component_register(
        SRCS
            "si_main.cpp"
            "driver/uart_data_broker.cpp" "driver/uart_data_broker.hpp"
            "storage/config_manager.cpp" "storage/config_manager.hpp"
            "misc/eternal.hpp"
            "protocol/chunk_codec.cpp" "protocol/chunk_codec.hpp"
            "protocol/config_handler.cpp" "protocol/config_handler.hpp"

        INCLUDE_DIRS
            "driver" "storage" "protocol" "misc"
)
