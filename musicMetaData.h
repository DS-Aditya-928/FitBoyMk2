typedef struct musicMeta_t{
  String trackName = "";
  String albumName = "";
  String artistName = "";

  long startTime;
  long startLoc;
  long length;

  bool playing = true;
} MusicMeta;