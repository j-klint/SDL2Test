# SDL2:n testailua

*Alustava versio. Toivon mukaan paranee tulevaisuudessa*

----------------------


Kirjoittelin suunnilleen tämmöisen n. v. 2021, vähän sen jälkeen, kun olin
aloittanut C++:n opettelun.

Olin lukenut huhuja, että [SDL2](https://www.libsdl.org/) olisi kelpo grafiikkakirjasto, jos C++:lla
haluaa grafiikoita tehdä, siispä päätin sitä kokeilla.

Olin katsellut yhtä Bisqwitin
[grafiikkaohjelmointivideota](https://youtu.be/HQYsFshbkYw?t=82), jonka
alussa hän selittää eri koordinaatistoista tai joistain sinne päin.
Ajattelin, että tuollainen pikku demonstraatiohan varmaan olisi hauska ja
melko nopeakin tehdä. Siispä yritin niin tehdä.

Olin lisäksi vast'ikään kuullut sellaisen
[huhun](https://www.youtube.com/watch?v=vQ60rFwh2ig), että
(*n+1*)-ulotteisilla matriiseilla/lineaarikuvauksilla voitaisiin hoitaa
*n*-ulotteisen avaruuden translaatiot ynnä lineaarikuvaukset.
Lineaarialgebrastahan olin oppinut, että kaikki äärellisulotteisten
avaruuksien lineaarikuvaukset voidaan hoitaa matriiseilla, ja
lineaarikuvausten yhdistämiset matriisien kertolaskuina. Mutta kun
lineaarikuvaus esim. kuvaa aina origon origoksi, niin translaatio ei voi olla
lineaarikuvaus. Opin sitten, että kun otetaan (*n+1*)-ulotteinen
lineaarikuvaus ja sen kuvat projisoidaan *n*:ään ulottuvuuteen, niin sillä on
mahdollista hoidella myös *n*-ulotteinen translaatio, ja vieläpä siten, että
kuvausten yhdistäminen edelleen toimii (*n+1*)-ulotteisten matriisien
kertolaskuna.

Sitä halusin testata. Havaintoni tästä harjoitustyöstä olivat, että hyvinpä
näyttää tämä konsti toimivan ainakin tapauksessa *n*&nbsp;=&nbsp;2.

Yhdelle kaverille kun esittelin, sanoi, että seuraavaksi [**collision
detection**](https://en.wikipedia.org/wiki/Collision_detection). Ensimmäisenä
ratkaisuideana minulle tuli mieleen, että kunhan liikkuvektori projisoidaan
vastaantulevalle seinälle jne., niin sillä hyvä, eli ihan sama idea kuin
[itse Bisqwitillä](https://www.youtube.com/watch?v=HQYsFshbkYw&t=997s). Mutta
siinä on vain semmoinen puute, että se toimii ainoastaan silloin, kun seinät
ovat kohtisuorassa toisiaan vastaan. Ihan samaan tapaan kuin
[Malusin](https://en.wikipedia.org/wiki/%C3%89tienne-Louis_Malus)
[laissa](https://en.wikipedia.org/wiki/Malus%27s_law), kun otetaan
projektioita pinnoille, jotka eivät ole kohtisuorassa toisiaan vastaan, niin
jotain aina jää jäljelle. Ja tämän jonkin avulla pystyy luikertelemaan
nurkista lävitse. Tämän saman [puutteen
myöntää](https://www.youtube.com/watch?v=HQYsFshbkYw&t=1106s) myös Bisqwit.

Yritin keksiä vedenpitävämpää keinoa ongelman ratkaisemiseksi. Olin onneksi
saanut paikalliselta ohjelmointigurulta lainaan Jason Gregoryn
[pelimoottorinteko-oppaan](https://www.gameenginebook.com/). Löysin siitä
vinkkejä, kuinka toteuttaa törmäyksenhoksaus. Moninkertaiset (2-ulotteiset)
*sphere castit* sitten tuli tehtyä, mutta nyt vaikuttaa toimivan.

Täytyy sanoa, että tämä "collision detection" oli kyllä ylivoimaisesti
vaikein ongelma tämän "teoksen" tekemisessä.
