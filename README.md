# SDL2:n testailua

## Käyttöohjeita (Windows)

Ensinnä pitää olla VSCoden ja C/C++-lisäkkeen lisäksi [kääntäjä asennettuna
sopivasti](https://code.visualstudio.com/docs/cpp/config-msvc). Eli VSCode
pitää käynnistää tuon *x64 Native Tools Command Prompt* -batin kautta, jotta
kääntäjän polku sun muut menisivät oikein.

Lisäksi pitää [SDL2](https://github.com/libsdl-org/SDL/releases/latest) olla
asennettuna tämän viereiseen hakemistoon. Ainakin minulla toimi, kun kopioin
tuon
[`SDL2-devel-2.30.6-VC.zip`](https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-devel-2.30.6-VC.zip)-paketin
sisällä olleen hakemiston sisällön hakemistoon `..\SDL2`. Siis niin, että
tiedostojen paikat täsmäävät tiedostoissa `.vscode\tasks.json` ja
`.vscode\c_cpp_properties.json` esiintyvien polkujen kanssa.

Sitten pitäisi kääntyä VSCodessa tuolla *build taskilla* `Kopioi SDL2.dll`,
jonka pitäisi kutsua myös varsinainen kääntämiskomento.


## Tarinointia

Kirjoittelin suunnilleen tämmöisen n. v. 2021, vähän sen jälkeen, kun olin
aloittanut C++:n opettelun.

Olin lukenut huhuja, että [SDL2](https://www.libsdl.org/) olisi kelpo
grafiikkakirjasto, jos C++:lla haluaa grafiikoita tehdä, siispä päätin sitä
kokeilla.

Olin katsellut yhtä Bisqwitin
[grafiikkaohjelmointivideota](https://youtu.be/HQYsFshbkYw?t=82), jonka
alussa hän selittää eri koordinaatistoista tai joistain sinne päin.
Ajattelin, että tuollainen pikku esittely, jossa on *wireframet* kolmesta eri
kuvakulmasta, varmaan olisi hauska ja melko nopeakin tehdä. Siispä yritin
niin tehdä.

Olin lisäksi vast'ikään kuullut sellaisen
[huhun](https://www.youtube.com/watch?v=vQ60rFwh2ig), että
(*n+1*)-ulotteisilla matriiseilla/lineaarikuvauksilla voitaisiin hoitaa
*n*-ulotteisen avaruuden translaatiot ynnä lineaarikuvaukset.
Lineaarialgebrastahan olin oppinut, että kaikki äärellisulotteisten
avaruuksien lineaarikuvaukset voidaan hoitaa matriiseilla, ja
lineaarikuvausten yhdistämiset niiden matriisien kertolaskuina. Mutta kun
lineaarikuvaus esim. kuvaa aina origon origoksi, niin translaatio ei
selvästikään voi olla lineaarikuvaus. Opin sitten, että kun otetaan
(*n+1*)-ulotteinen lineaarikuvaus ja sen kuvat projisoidaan *n*:ään
ulottuvuuteen, niin sillä on mahdollista hoidella myös *n*-ulotteinen
translaatio, ja kuvausten yhdistäminen edelleen toimii mainiosti
(*n+1*)-ulotteisten matriisien kertolaskuna.

Sitä halusin testata. Havaintoni tästä harjoitustyöstä olivat, että hyvinpä
näyttää tämä konsti toimivan ainakin tapauksessa *n*&nbsp;=&nbsp;2.

Yhdelle kaverille kun esittelin, sanoi, että seuraavaksi [collision
detection](https://en.wikipedia.org/wiki/Collision_detection). Ensimmäisenä
ratkaisuideana minulle tuli mieleen, että kunhan liikkuvektori projisoidaan
vastaantulevalle seinälle jne., niin sillä hyvä, eli vähän sama idea kuin
[itse Bisqwitillä](https://www.youtube.com/watch?v=HQYsFshbkYw&t=997s). Mutta
siinä on vain semmoinen puute, että se toimii ainoastaan silloin, kun seinät
ovat kohtisuorassa toisiaan vastaan. Ihan samaan tapaan kuin [Malusin
laissa](https://en.wikipedia.org/wiki/Malus%27s_law), kun otetaan perättäisiä
projektioita tasoille, jotka eivät ole kohtisuorassa toisiaan vastaan, niin
jotain aina jää jäljelle. Ja tämän jonkin avulla pystyy luikertelemaan
nurkista lävitse. Tähän samaan puutteeseen tuntuu myös Bisqwit
[viittaavan](https://www.youtube.com/watch?v=HQYsFshbkYw&t=1106s).

Yritin keksiä vedenpitävämpää keinoa ongelman ratkaisemiseksi. Olin onneksi
saanut paikalliselta ohjelmointigurulta lainaan Jason Gregoryn
[pelimoottorinteko-oppaan](https://www.gameenginebook.com/). Löysin siitä
vinkkejä, kuinka toteuttaa törmäyksenhoksaus. Moninkertaiset (2-ulotteiset)
*sphere castit* sitten tuli tehtyä. Ei tullut koodista kovin sievää, mutta
nyt vaikuttaa toimivan.

Täytyy sanoa, että tämä "collision detection" oli kyllä ylivoimaisesti
vaikein ongelma tämän tekemisessä.
