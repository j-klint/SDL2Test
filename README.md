# SDL2:n testailua

Tämä on harjoitustyö, jossa testailin [SDL2:ta](https://libsdl.org/) sekä
matriisien käyttökelpoisuutta translaatioiden hoitamisessa pelkkien
lineaarikuvausten lisäksi, sekä myös joitain 2-ulotteisia törmäyksenhoksaus
ja -hoitelukonsteja. Piirtelee ruudulle pikku rautalankahäkkyröitä.

`W`&nbsp;`A`&nbsp;`S`&nbsp;`D`&nbsp;:illa voi siirrellä "pelaajahahmoa",
`Q`:lla ja `E`:llä käännellä ja `Shift` sekä `Ctrl` ovat turbo- ja
hitailunapit. Liikkeet näkyvät mukamas kolmesta eri kuvakulmasta. Command line
-argumenttina voi syöttää törmäystarkistuksissa käytettävän pelaajahahmon
säteen pikseleissä.

Suuri inspiraatio tähän oli [juutuubaaja Bisqwit](https://www.youtube.com/@Bisqwit). Enemmän
taustatarinaa tälle räpellykselleni löytynee tuolta [Tarinointia-osiosta](#tarinointia).


## Käyttöohjeita

tl;dr: Kloonatkaa repo, katsokaa, että SDL2 on kondiksessa, kääntäkää ja
linkatkaa `*.cpp`.

Tämä ompi
[VSCode](https://en.wikipedia.org/wiki/Visual_Studio_Code)-projekti, ja koska
VSCodessa ei tule kääntäjää mukana, niin sellainen on asennettava muualta.
Seuraavassa vinkkejä.

### Windows

1. **Asenna kääntäjä ja tarpeellinen Windows SDK.**

    Olin sen verran laiska, että kun kerran minulla Visual Studio oli jo
    asennettuna, niin en viitsinyt sen lisäksi ruveta Windowsille enempiä
    kääntäjiä asentelemaan. Tässä neuvoja, kuinka MSVC-kääntäjän saanee
    toimimaan VSCoden kanssa. Se saattaa vaatia vähän jippoja. Tässä
    viralliset neuvot:

    <https://code.visualstudio.com/docs/cpp/config-msvc>

    Ja niille, jotka ovat vielä laiskempia kuin minä, tässä on vielä sama
    suomeksi tavattuna:

    * Jos on jo Visual Studio asennettuna, niin valitse sieltä Visual Studio
      Installerista "Desktop development with C++".

    * Jos ei ole, eikä tahdo koko Visual Studiota asentaa, niin
      [downloads-sivulta](https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022)
      löytyy tuommoinen kuin "Build Tools for Visual Studio 2022". Sillä saa
      toivon mukaan tarvittavat osat asennettua ilman Visual Studiota

    * Joka tapauksessa komponentit, jotka vähintäänkin olisi asennettava,
      ovat *MSVC blaablaablaa*, jossa on `cl.exe`-kääntäjä mukana, sekä
      *Windows blaa SDK (blaablaa)*, jossa on mukana Windows-ohjelmille
      tuikitärkeitä headereitä ja kirjastoja yms.

    Joka ei halua MSVC:n kanssa ruveta leikkimään, voi vaikka yrittää käyttää
    vaikka [MingW:tä](https://code.visualstudio.com/docs/cpp/config-mingw),
    mutta sitä en ole itse ikinä testannut, joten siinä asiassa en osaa
    neuvoa.

1. **"Asenna" SDL2 tämän projektin viereiseen hakemistoon.**

    * Uusin versio: <https://github.com/libsdl-org/SDL/releases/latest>

        * Noista kannattaa valita paketti
          [`SDL2-devel-*-VC.zip`](https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-devel-2.30.6-VC.zip)
          eli tuo `-VC.zip`-loppuinen.

    * Kopioi puretun paketin sisällä olleen hakemiston sisältö tämän
      projektin viereiseen `SDL2`-nimiseen hakemistoon. Eli suhteellisena
      polkuna `..\SDL2`:een.

    * Jos edellinen kohta oli liian epäselvästi sanottu, niin voit varmistaa,
      että SDL2-tiedostojen sijainnit vastaavat `.vscode\tasks.json` ja
      `.vscode\c_cpp_properties.json` -tiedostoissa määriteltyjä polkuja.

    * Tai voihan sen muuallekin panna, kunhan korjaa noista kahdesta em.
      tiedostosta polut.

1. **Käännä sovellus.**

    * Jos käytät MSVC:tä, niin käynnistä VSCode ensin käynnistämällä *x64 Native Tools Command Prompt* -komentokehote (löytyy Windowsin starttimenusta, jos kohdan 1 asennus onnistui normaaliin tapaan) ja sitten avaamalla projektihakemisto VSCodella siinä *Command Prompt*issa, eli suunnista `cd`:llä tai `pushd`:llä kys. hakemistoon ja kirjoita:

      ```cmd
      code .
      ```

      Tämä toivon mukaan varmistaa, että kääntäjän polut ym. ovat oikein.

    * Valitse VSCoden “Tasks”-näkymä (`Ctrl+Shift+B`).

    * Etsi ja valitse `Kopioi SDL2.dll (Windows)` -tehtävä. Tämän pitäisi
      ensin kääntää ohjelma ja sitten kopioda vaadittava dll samaan
      hakemistoon exe:n kanssa.

    * `.exe`- ja `.dll`-tiedostojen pitäisi nyt löytyä
      `built`-alihakemistosta.


### GNU/Linux

Esimerkkikomennot ovat Ubuntun/Debianin tyyliin. Muissa distroissa saattavat
paketinhallintaohjelmat ja pakettien nimet ym.:t olla erit.

Pitää olla kääntäjä (`g++`) asennettuna:

```bash
sudo apt install g++
```

Pitää myös olla SDL2:

```bash
sudo apt install libsdl2-dev
```

Sitten kun on projektihakemisto avattu VSCodiumissa, niin *build-taskilla*
(`Ctrl+Shift+B`) nimeltä `Käännä ohjelma` pitäisi ohjelman kääntyä ja
*executablen* (mikähän tämä sitten onkaan suomeksi) päätyä alihakemistoon
nimeltä `built`.

Tai jos ei ole VSCodiumia asennettuna, niin voi ihan vain ajaa
projektihakemistossa komennon

```bash
.vscode/linuxbuild.bash
```

Niin tuo *build-task*:kin tekee.


## Tarinointia

Kirjoittelin suunnilleen tämmöisen n. v. 2021, vähän sen jälkeen, kun olin
aloittanut C++:n opettelun.

Olin lukenut huhuja, että [SDL2](https://www.libsdl.org/) olisi kelpo
grafiikkakirjasto, jos C++:lla haluaa grafiikoita tehdä. Siispä päätin sitä
kokeilla.

Olin katsellut yhtä Bisqwitin
[grafiikkaohjelmointivideota](https://youtu.be/HQYsFshbkYw?t=82), jonka
alussa hän selittää eri koordinaatistoista tai joistain sinne päin.
Ajattelin, että tuollainen pikku esittely, jossa on *wireframet* kolmesta eri
kuvakulmasta, varmaan olisi hauska ja melko nopeakin tehdä. Siispä yritin
niin tehdä.

Olin lisäksi vast'ikään kuullut sellaisen
[huhun](https://www.youtube.com/watch?v=vQ60rFwh2ig), että
(*n+1*)-ulotteisilla matriiseilla voitaisiin hoitaa *n*-ulotteisessa
avaruudessa ei ainoastaan lieaarikuvaukset vaan myös translaatiot.
Lineaarialgebrastahan olin oppinut, että kaikki äärellisulotteisten
avaruuksien lineaarikuvaukset voidaan kyllä hoitaa matriiseilla, ja
lineaarikuvausten yhdistämiset niiden matriisien kertolaskuina. Mutta kun
lineaarikuvaus esim. kuvaa aina origon origoksi, niin translaatio ei
selvästikään voi olla lineaarikuvaus. Opin sitten, että kun otetaan
(*n+1*)-ulotteinen lineaarikuvaus ja sen kuvat projisoidaan *n*:ään
ulottuvuuteen helpoimmalla mahdollisella tavalla, niin sillä on mahdollista
hoidella myös *n*-ulotteinen translaatio, ja kuvausten yhdistäminen edelleen
toimii mainiosti (*n+1*)-ulotteisten matriisien kertolaskuna.

Sitä halusin testata. Havaintoni tästä harjoitustyöstä oli, että hyvinpä
näyttää tämä konsti toimivan ainakin tapauksessa *n*&nbsp;=&nbsp;2.

Yhdelle kaverille kun esittelin, sanoi, että seuraavaksi
["clipping"](https://en.wikipedia.org/wiki/Collision_detection). Ensimmäisenä
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
nyt vaikuttaa sentään toimivan.

Täytyy sanoa, että tämä "collision detection" oli kyllä ylivoimaisesti
vaikein ongelma tämän tekemisessä.
