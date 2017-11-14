#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/song.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

namespace tempo
{
	Song::Song(std::string path)
	{
		buffer.loadFromFile(path);
		sound.setBuffer(buffer);
	}

	void Song::start()
	{
		sound.play();
	}

	void Song::pause()
	{
		sound.pause();
	}

	void Song::stop()
	{
		sound.stop();
	}

	void Song::skip(sf::Time delta)
	{
		sound.setPlayingOffset(sound.getPlayingOffset() + delta);
	}

	void Song::set_volume(float volume)
	{
		sound.setVolume(volume);
	}

	void Song::inc_volume(float volume)
	{
		float vol = sound.getVolume();
		sound.setVolume(std::min(vol + volume, 100.f));
	}

	void Song::dec_volume(float volume)
	{
		float vol = sound.getVolume();
		sound.setVolume(std::max(vol - volume, 0.f));
	}
}
