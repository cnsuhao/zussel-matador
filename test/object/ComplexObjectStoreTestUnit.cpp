#include "ComplexObjectStoreTestUnit.hpp"

#include "object/object_ptr.hpp"
#include "object/object_exception.hpp"
#include "object/object_view.hpp"
#include "object/object_serializer.hpp"

#include "Track.hpp"
#include "Album.hpp"
#include "Artist.hpp"

#include <iostream>

using namespace oos;
using namespace std;

class Item : public object_list_node<Item>
{
public:
  Item() {}
  Item(const std::string &name) : name_(name) {}
  virtual ~Item() {}

	void read_from(object_atomizer *reader)
  {
    object_list_node::read_from(reader);
    reader->read_string("name", name_);
  }
	void write_to(object_atomizer *writer)
  {
    object_list_node::write_to(writer);
    writer->write_string("name", name_);
  }
  
  std::string name() const { return name_; }
  void name(const std::string &n) { name_ = n; }

private:
  std::string name_;
};

typedef object_ref_list_node<Track> AlbumTrack;

ComplexObjectStoreTestUnit::ComplexObjectStoreTestUnit()
  : unit_test("complex objectstore")
{
  add_test("first", std::tr1::bind(&ComplexObjectStoreTestUnit::first, this), "first complex test");
  add_test("itemlist", std::tr1::bind(&ComplexObjectStoreTestUnit::item_list, this), "item list test");
  add_test("album", std::tr1::bind(&ComplexObjectStoreTestUnit::album, this), "first complex test");
  add_test("serializer", std::tr1::bind(&ComplexObjectStoreTestUnit::serializer, this), "first complex test");
}

void
ComplexObjectStoreTestUnit::initialize()
{
  ostore_.insert_prototype(new object_producer<Item>, "ITEM");
  ostore_.insert_prototype(new object_producer<Artist>, "ARTIST");
  ostore_.insert_prototype(new object_producer<Track>, "TRACK");
  ostore_.insert_prototype(new object_producer<Album>, "ALBUM");
  ostore_.insert_prototype(new object_producer<AlbumTrack>, "ALBUMTRACK");
  ostore_.insert_prototype(new object_producer<MediaTrack>, "MEDIATRACK", "TRACK");
  ostore_.insert_prototype(new object_producer<AudioTrack>, "AUDIOTRACK", "MEDIATRACK");
  ostore_.insert_prototype(new object_producer<VideoTrack>, "VIDEOTRACK", "MEDIATRACK");
}

void
ComplexObjectStoreTestUnit::finalize()
{
  ostore_.clear();
}

void
ComplexObjectStoreTestUnit::first()
{
  ostore_.insert(new Album);
  ostore_.insert(new Album);
  ostore_.insert(new Track);
  ostore_.insert(new AudioTrack);
  ostore_.insert(new VideoTrack);
  ostore_.insert(new Artist);
  ostore_.insert(new VideoTrack);
  // show object hierarchy


  Track *track = NULL;
  for (int i = 0; i< 5; ++i) {
    track = new Track;
    if (!track) {
      cout << "couldn't create object of type 'TRACK'\n";
      return;
    }
    ostore_.insert(track);
  }
  Artist *artist = NULL;
  for (int i = 0; i< 5; ++i) {
    artist = new Artist;
    if (!artist) {
      cout << "couldn't create object of type 'TRACK'\n";
      return;
    }
    ostore_.insert(artist);
  }
  for (int i = 0; i< 5; ++i) {
    track = new Track;
    if (!track) {
      cout << "couldn't create object of type 'TRACK'\n";
      return;
    }
    ostore_.insert(track);
  }
  // show object hierarchy
  ostore_.dump_prototypes(cout);

  ostore_.dump_objects(cout);

  {
    object_view<Track> trackview(ostore_, true);

    object_view<Track>::const_iterator first = trackview.begin();
    object_view<Track>::const_iterator last = trackview.end();

    cout << "Show all tracks (excluding siblings):\n";
    while (first != last) {
      object_ptr<Track> optr = (*first++);
      cout << "Track [" << *optr.get() << "]\n";
    }
    
    trackview.skip_siblings(false);
    first = trackview.begin();
    last = trackview.end();
    cout << "Show only tracks (including siblings):\n";
    while (first != last) {
      object_ptr<Track> optr = (*first++);
      cout << "Track [" << *optr.get() << "]\n";
    }
    cout << "Removing first track.\n"; 
    typedef object_ptr<Track> track_ptr;
    track_ptr t = trackview.begin().optr();

    ostore_.remove(t);

    first = trackview.begin();
    last = trackview.end();
    cout << "Show only tracks (including siblings):\n";
    while (first != last) {
      object_ptr<Track> optr = (*first++);
      cout << "Track [" << *optr.get() << "]\n";
    }

    trackview.skip_siblings(true);
    cout << "Removing last track.\n"; 
    typedef object_ptr<Track> track_ptr;
    t = (--trackview.end()).optr();

    ostore_.remove(t);

    ostore_.dump_objects(std::cout);
    ostore_.dump_prototypes(std::cout);
  }
  {
    object_view<VideoTrack> trackview(ostore_);
    object_view<VideoTrack>::const_iterator first = trackview.begin();
    object_view<VideoTrack>::const_iterator last = trackview.end();

    cout << "Show all video tracks:\n";
    while (first != last) {
      object_ptr<VideoTrack> optr = (*first++);
      cout << "VideoTrack [" << *optr.get() << "]\n";
    }
  }
  {
    object_view<AudioTrack> atrackview(ostore_);
    object_view<AudioTrack>::const_iterator first = atrackview.begin();
    object_view<AudioTrack>::const_iterator last = atrackview.end();

    cout << "Show all audio tracks:\n";
    while (first != last) {
      object_ptr<AudioTrack> optr = (*first++);
      cout << "AudioTrack [" << *optr.get() << "]\n";
    }
  }
  {
    object_view<Artist> artistview(ostore_);
    object_view<Artist>::const_iterator first = artistview.begin();
    object_view<Artist>::const_iterator last = artistview.end();

    cout << "Show all artists:\n";
    while (first != last) {
      object_ptr<Artist> optr = (*first++);
      cout << "Artist [" << *optr.get() << "]\n";
    }
  }
  /*
  typedef object_view<Track> TrackView;
  const TrackView track_view(ostore_);
  
  if (track_view.empty()) {
    cout << "no tracks!\n";
  } else {
    cout << "tracks available!\n";
    TrackView::const_iterator first = track_view.begin();
    TrackView::const_iterator last = track_view.end();
    while (first != last) {
      cout << "Track #" << (*first++)->id() << "\n";
    }
    object_ptr<Track> track1 = track_view.begin().optr();
    object_ptr<Track> track2 = track1;
    track1->delete_object();
  }
  
  object_view<Album> album_view(ostore_);
  
  if (album_view.empty()) {
    cout << "no albums!\n";
  } else {
    cout << "albums available!\n";
  }
  */
}

void
ComplexObjectStoreTestUnit::item_list()
{
  typedef object_list<Item> ItemList;
  typedef object_ptr<Artist> artist_ptr;

  artist_ptr acdc = ostore_.insert(new Artist("AC/DC"));
  artist_ptr genesis = ostore_.insert(new Artist("Genesis"));
  ostore_.dump_prototypes(std::cout);
  ItemList ilist;
  //ostore_.insert(ilist);

  /*
  ilist.push_back(new Item("Schrank"));
  ilist.push_back(new Item("Tisch"));
  ilist.push_back(new Item("Stuhl"));
  ilist.push_back(new Item("Bett"));
  */
  //ilist.push_front(new Item("Teppich"));

  ostore_.dump_objects(std::cout);
  ostore_.dump_prototypes(std::cout);

  std::cout << "removing artist [" << genesis->name() << "] (" << genesis.get() << ")\n";
  ostore_.remove(genesis);
  ostore_.dump_prototypes(std::cout);
  std::cout << "removing artist [" << acdc->name() << "] (" << acdc.get() << ")\n";
  ostore_.remove(acdc);
  ostore_.dump_prototypes(std::cout);

  ItemList::const_iterator first = ilist.begin();
  ItemList::const_iterator last = ilist.end();

  while (first != last) {
    cout << "item name: " << first->name() << "\n";
    ++first;
  }
  
  ostore_.remove_prototype("ARTIST");
}

void
ComplexObjectStoreTestUnit::album()
{
  typedef object_ptr<Artist> artist_ptr;
  typedef object_ptr<Track> track_ptr;
  typedef object_ptr<Album> album_ptr;
  artist_ptr artist = ostore_.insert(new Artist("Chris Huelsbeck"));
  album_ptr album = ostore_.insert(new Album("symphonic shades", artist));
  
  track_ptr track = ostore_.insert(new Track(1, "Grand Monster Slam (Opening Fanfare)", 165));
  album->add(track);
  track = ostore_.insert(new Track(2, "X-Out (Main Theme)", 311));
  album->add(track);
  track = ostore_.insert(new Track(3, "Jim Power in Mutant Planet (Main Theme)", 337));
  album->add(track);
  track = ostore_.insert(new Track(4, "Tower of Babel", 279));
  album->add(track);
  track = ostore_.insert(new Track(5, "Turrican 3 - Payment Day (Piano Suite)", 318));
  album->add(track);
  track = ostore_.insert(new Track(6, "Gem'X (Main Theme)", 220));
  album->add(track);
  track = ostore_.insert(new Track(7, "Apidya II (Suite)", 325));
  album->add(track);
  track = ostore_.insert(new Track(8, "R-Type (Main Theme)", 318));
  album->add(track);
  track = ostore_.insert(new Track(9, "Licht am Ende des Tunnels (Suite)", 309));
  album->add(track);
  track = ostore_.insert(new Track(10, "The Great Giana Sisters (Suite)", 330));
  album->add(track);
  track = ostore_.insert(new Track(11, "Rony Barrak-Solo", 305));
  album->add(track);
  track = ostore_.insert(new Track(12, "Tunnel B1 (Suite)", 298));
  album->add(track);
  track = ostore_.insert(new Track(13, "Symphonic Shades", 288));
  album->add(track);
  track = ostore_.insert(new Track(14, "Karawane der Elefanten", 251));
  album->add(track);
  track = ostore_.insert(new Track(15, "Turrican II - The Final Fight (Renderings: Main Theme)", 552));
  album->add(track);
  
  object_view<Track> trackview(ostore_, true);

  object_view<Track>::const_iterator first = trackview.begin();
  object_view<Track>::const_iterator last = trackview.end();

  cout << "Show all tracks (excluding siblings):\n";
  while (first != last) {
    object_ptr<Track> optr = (*first++);
    cout << "Track: " << optr->title() << "\n";
  }

  Album::const_iterator afirst = album->begin();
  Album::const_iterator alast = album->end();

  typedef object_ref<Track> track_ref;

  cout << "Show all tracks of album [" << album->name() << "]:\n";
  while (afirst != alast) {
    track_ref tref = (*afirst++)->oref();
    cout << "Track: " << tref->title() << " (Artist: " << tref->artist()->name() << ")\n";
  }
}

void
ComplexObjectStoreTestUnit::serializer()
{
  Track *track = new Track(1, "Hallo Welt", 300);
  
  object_serializer serializer;
  
  serializer.serialize(track);
  
  delete track;
  
  track = new Track();
  
  serializer.deserialize(track);

  cout << "track title: " << track->title() << "\n";
}