poke: poke.cpp
	@echo "Compiling..."
	@g++ -Wall -Werror poke.cpp \
	data_structures/priority_queue/priority_queue.cpp \
	data_structures/queue/queue.cpp \
	data_structures/random/random.cpp \
	dijkstra_algorithms/dijkstra_algorithms.cpp \
	global_variables/global_variables.cpp \
	terrain_generation/terrain_generation.cpp \
	character/npc/npc.cpp \
	character/player/player.cpp \
	game/file_parsing/file.cpp \
	game/file_parsing/csv_classes/helper_functions/helper_functions_for_csv.cpp \
	game/file_parsing/csv_classes/pokemon_stats/pokemon_stats.cpp \
	game/file_parsing/csv_classes/pokemon_types/pokemon_types.cpp \
	game/file_parsing/csv_classes/experience/experience.cpp \
	game/file_parsing/csv_classes/stats/stats.cpp \
	game/file_parsing/csv_classes/pokemon/pokemon.cpp \
	game/file_parsing/csv_classes/moves/moves.cpp \
	game/file_parsing/csv_classes/pokemon_moves/pokemon_moves.cpp \
	game/file_parsing/csv_classes/pokemon_species/pokemon_species.cpp \
	game/file_parsing/csv_classes/type_names/type_names.cpp \
	game/game.cpp -lm -lncurses -o poke
	@echo "Done. Type [./poke] to start."
	@echo "For all commands type, [./poke help]"

clean:
	@echo "Removed poke."
	@rm -f poke