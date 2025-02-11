#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hangman_utils.h"

void Hungman(int lives)
{
    switch (lives)
    {
    case 8:
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("=========\n");

        break;
    case 7:

        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 6:

        printf("  +---+\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 5:

        printf("  +---+\n");
        printf("  |   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;

    case 4:

        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 3:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf("  |   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 2:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 1:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf(" /    |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 0:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf(" / \\  |\n");
        printf("      |\n");
        printf("💀 GAME OVER!\n");
        printf("=========\n");

        break;

        break;
    }
}

char *randomWord()
{

    char *selectedWord;
    char *words[] = {
        "mountain", "river", "ocean", "forest", "desert", "valley", "prairie", "jungle", "tundra", "volcano",
        "island", "beach", "harbor", "canyon", "plateau", "summit", "glacier", "cliff", "waterfall", "horizon",
        "sunrise", "sunset", "thunder", "lightning", "rainbow", "whirlpool", "sandstorm", "tornado", "avalanche", "earthquake",
        "meadow", "garden", "orchard", "vineyard", "pasture", "farmland", "wilderness", "grove", "swamp", "marsh",
        "school", "college", "library", "museum", "gallery", "stadium", "theater", "hospital", "station", "university",
        "airplane", "helicopter", "submarine", "scooter", "bicycle", "motorcycle", "bus", "tram", "subway", "train",
        "police", "fireman", "doctor", "nurse", "teacher", "lawyer", "judge", "pilot", "engineer", "scientist",
        "artist", "musician", "painter", "sculptor", "writer", "author", "director", "actor", "dancer", "singer",
        "student", "professor", "librarian", "manager", "worker", "clerk", "cashier", "waiter", "barista", "chef",
        "computer", "keyboard", "monitor", "printer", "scanner", "router", "modem", "speaker", "tablet", "camera",
        "mountain", "desert", "island", "valley", "forest", "river", "prairie", "ocean", "tundra", "waterfall",
        "software", "hardware", "network", "database", "browser", "program", "system", "server", "backup", "virtual",
        "python", "java", "csharp", "golang", "kotlin", "swift", "binary", "array", "vector", "pointer",
        "fiction", "novel", "poetry", "drama", "comedy", "tragedy", "biography", "mystery", "fantasy", "romance",
        "justice", "freedom", "honesty", "integrity", "loyalty", "compassion", "patience", "courage", "respect", "wisdom",
        "biology", "chemistry", "physics", "geology", "astronomy", "botany", "zoology", "ecology", "genetics", "microbes",
        "algorithm", "equation", "formula", "theorem", "calculus", "geometry", "algebra", "statistics", "integral", "matrix",
        "robotics", "cybernetics", "nanotech", "quantum", "gravity", "relativity", "telescope", "microscope", "satellite", "probe",
        "horizon", "galaxy", "planet", "comet", "asteroid", "meteor", "nebula", "quasar", "pulsar", "blackhole",
        "stadium", "ballpark", "court", "arena", "gym", "track", "field", "rink", "pool", "raceway",
        "soccer", "basketball", "baseball", "football", "hockey", "volleyball", "tennis", "cricket", "rugby", "golf",
        "ballet", "opera", "concert", "festival", "parade", "exhibit", "circus", "performance", "competition", "audition",
        "guitar", "piano", "violin", "drums", "trumpet", "saxophone", "flute", "cello", "trombone", "clarinet",
        "imagine", "create", "invent", "design", "solve", "analyze", "explore", "discover", "develop", "build",
        "dialogue", "character", "setting", "theme", "plot", "conflict", "climax", "resolution", "narrative", "scene",
        "problem", "solution", "method", "process", "hypothesis", "experiment", "result", "conclusion", "evidence", "data",
        "dinosaur", "mammal", "reptile", "insect", "amphibian", "species", "organism", "ecosystem", "habitat", "predator",
        "economy", "market", "currency", "finance", "investment", "trade", "industry", "business", "capital", "taxes",
        "republic", "monarchy", "democracy", "dictator", "senator", "president", "governor", "mayor", "minister", "judge",
        "culture", "society", "community", "tradition", "ritual", "custom", "language", "religion", "belief", "values",
        "history", "timeline", "dynasty", "empire", "kingdom", "revolution", "warfare", "battle", "treaty", "independence",
        "program", "project", "assignment", "task", "deadline", "goal", "strategy", "meeting", "discussion", "plan",
        "robot", "drone", "machine", "automation", "sensor", "microchip", "circuit", "gadget", "interface", "controller",
        "company", "startup", "corporation", "agency", "bureau", "office", "branch", "firm", "subsidiary", "enterprise",
        "resource", "supply", "distribution", "demand", "management", "inventory", "production", "operation", "maintenance", "logistics",
        "leader", "team", "group", "collaborate", "negotiate", "coordinate", "support", "assist", "consult", "evaluate",
        "website", "blog", "forum", "social", "platform", "media", "application", "content", "service", "support",
        "earth", "planet", "mars", "venus", "jupiter", "saturn", "mercury", "uranus", "neptune", "pluto",
        "apple", "banana", "grapes", "orange", "melon", "mango", "peach", "cherry", "pear", "plum",
        "dream", "imagine", "create", "wonder", "discover", "explore", "build", "invent", "learn", "grow",
        "smile", "laugh", "cry", "sigh", "yawn", "shout", "whisper", "scream", "talk", "sing",
        "run", "jump", "walk", "dance", "swim", "climb", "crawl", "slide", "stretch", "spin",
        "shirt", "pants", "jacket", "scarf", "gloves", "hat", "shoes", "socks", "belt", "boots",
        "phone", "tablet", "laptop", "camera", "remote", "speaker", "headphones", "battery", "charger", "monitor",
        "pencil", "eraser", "marker", "notebook", "ruler", "scissors", "glue", "tape", "paper", "folder",
        "river", "lake", "stream", "pond", "waterfall", "spring", "bay", "ocean", "sea", "fjord",
        "cloud", "rain", "snow", "hail", "wind", "fog", "storm", "lightning", "thunder", "breeze",
        "sun", "moon", "stars", "planet", "comet", "meteor", "asteroid", "galaxy", "universe", "space"};
}