# Bolide

Projekt gry wyścigowej 3D wykonanej w języku **C++** z wykorzystaniem **OpenGL**. Celem projektu jest stworzenie środowiska, w którym można sterować bolidem Formuły 1 poruszającym się po odwzorowanym torze **Monza**.

Projekt jest rozwijany jako aplikacja desktopowa dla systemu Windows w środowisku Visual Studio.

## 🎮 Aktualne możliwości

W obecnej wersji projektu dostępne są m.in.:

- renderowanie sceny 3D z wykorzystaniem OpenGL,
- odwzorowany tor Monza,
- model 3D bolidu Formuły 1,
- tekstury i materiały modeli,
- skybox otaczający scenę,
- oświetlenie kierunkowe,
- mapowanie cieni (shadow mapping),
- obsługa głębi sceny (depth testing),
- przezroczystość obiektów z wykorzystaniem blendingu,
- ruch bolidu zależny od jego aktualnego kierunku obrotu,
- przyspieszanie i hamowanie bolidu,
- ograniczenie maksymalnej prędkości,
- tarcie powodujące stopniowe wytracanie prędkości,
- skręcanie bolidu podczas jazdy,
- możliwość zatrzymania bolidu,
- kilka trybów pracy kamery.

## 📷 Wygląd gry

### Widok toru

> **Miejsce na zdjęcie:**
>
> `![Widok toru](images/gameplay_track.png)`

### Widok bolidu

> **Miejsce na zdjęcie:**
>
> `![Widok bolidu](images/gameplay_bolide.png)`

### Widok z kokpitu

> **Miejsce na zdjęcie:**
>
> `![Widok z kokpitu](images/gameplay_driver.png)`

### Dodatkowy zrzut ekranu

> **Miejsce na zdjęcie:**
>
> `![Gameplay](images/gameplay.png)`

Zdjęcia można dodać później do katalogu `images/` w repozytorium.

## 🎥 Tryby kamery

Kamera może pracować w kilku trybach:

| Klawisz | Tryb |
|---|---|
| `0` | Swobodna kamera |
| `1` | Kamera z perspektywy głowy kierowcy |
| `2` | Kamera za bolidem |
| `3` | Kamera przed bolidem |

### Kamera swobodna

W trybie swobodnym kamera może być przemieszczana po scenie, a kierunek patrzenia jest sterowany za pomocą myszy przytrzymującej prawy przycisk.

## 🏎️ Sterowanie bolidem

| Klawisz | Działanie |
|---|---|
| `↑` | Przyspieszanie |
| `↓` | Hamowanie / jazda do tyłu |
| `←` | Skręt w lewo |
| `→` | Skręt w prawo |
| `P` | Zatrzymanie bolidu |
| `ESC` | Zamknięcie programu |

Ruch bolidu jest obliczany względem jego aktualnej orientacji, dzięki czemu przyspieszanie powoduje ruch w kierunku, w którym zwrócony jest bolid.

## 🛠️ Technologie i biblioteki

Projekt wykorzystuje:

- **C++20**,
- **OpenGL** – renderowanie grafiki 3D,
- **GLFW** – obsługa okna, klawiatury i myszy,
- **GLEW** – dostęp do rozszerzeń OpenGL,
- **GLM** – operacje matematyczne i transformacje 3D,
- **Assimp** – import modeli 3D,
- **SOIL2** – obsługa tekstur,
- **GLSL** – programowanie shaderów wierzchołków i fragmentów.

Biblioteki znajdują się w katalogu `Dependencies/`.

## 📁 Struktura projektu

```text
Bolide/
├── Bolide.slnx
├── Bolide/
│   ├── inc/                 # Pliki nagłówkowe klas i komponentów
│   ├── src/                 # Kod źródłowy aplikacji
│   │   └── main.cpp
│   ├── shaders_config/      # Shadery GLSL
│   └── Bolide.vcxproj       # Konfiguracja projektu Visual Studio
├── Dependencies/            # Biblioteki zewnętrzne
└── README.md
```

Najważniejsze elementy projektu obejmują m.in. klasy odpowiedzialne za kamerę, shadery, modele 3D, tekstury, skybox oraz cienie.

## ▶️ Uruchomienie projektu

1. Sklonuj repozytorium.
2. Otwórz plik `Bolide.slnx` w Visual Studio.
3. Upewnij się, że wybrana jest konfiguracja `x64`.
4. Zbuduj projekt.
5. Uruchom aplikację.

Projekt jest skonfigurowany dla **C++20** i wykorzystuje biblioteki znajdujące się w katalogu `Dependencies/`.

## 🌍 Scena 3D

Główną scenę stanowi model toru **Monza**, na którym umieszczony został model bolidu Formuły 1. Scena wykorzystuje osobne shadery dla głównych obiektów oraz skyboxa. Cienie są generowane w osobnym przebiegu renderowania z wykorzystaniem mapy głębokości.

## 🚧 Dalszy rozwój projektu

Kolejnym planowanym etapem rozwoju projektu jest **dodanie grawitacji i fizyki ruchu bolidu**.

Grawitacja będzie szczególnie istotna ze względu na to, że tor nie znajduje się na jednej wysokości. Docelowo ruch bolidu powinien uwzględniać ukształtowanie toru, a nie tylko przesuwać model po płaszczyźnie XZ.

Planowane są również dalsze usprawnienia fizyki oraz zachowania bolidu, aby jego ruch był bardziej realistyczny.

## 📌 Status projektu

Projekt jest w trakcie rozwoju. Podstawowy system renderowania sceny, obsługa modeli 3D, kamera oraz podstawowy model ruchu bolidu są już zaimplementowane. Kolejne prace koncentrują się na rozbudowie fizyki i poprawie realizmu jazdy.
