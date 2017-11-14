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

}
