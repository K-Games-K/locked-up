#pragma once

#include <google/protobuf/message.h>
#include <google/protobuf/any.pb.h>

#include "clue_found_packet.pb.h"
#include "countdown_packet.pb.h"
#include "debug_packet.pb.h"
#include "disconnect_packet.pb.h"
#include "fake_clue_packet.pb.h"
#include "game_board_packet.pb.h"
#include "game_results_packet.pb.h"
#include "game_start_packet.pb.h"
#include "join_game_packet.pb.h"
#include "murderer_packet.pb.h"
#include "new_turn_packet.pb.h"
#include "player_move_packet.pb.h"
#include "player_ready_packet.pb.h"
#include "players_list_packet.pb.h"
#include "vote_packet.pb.h"

namespace Packet
{
    using Packet = google::protobuf::Message;   // `Packet` is a polymorphic type that represents any packet.

    using Any = google::protobuf::Any;  // `Any` allows checking which packet type it holds, and unpacking it
                                        // into appropriate `Packet` object.
}