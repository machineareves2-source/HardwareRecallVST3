# Hardware Recall VST3 — bibliothèque intégrale v0.3 (source)

## Inclus
- **203 références physiques, 229 unités individuelles** extraites de `inventaire_materiel_audiofanzine_estimation_2026.xlsx` (octobre 2026). L'inventaire comprend aussi des micros, instruments et accessoires sans nécessairement de façade à rappeler.
- Toutes les unités sont sélectionnables dans le plugin, chaque exemplaire ayant des positions indépendantes.
- Sauvegarde des paramètres dans l'état du VST3, donc dans les projets REAPER.
- **Trois familles de façades déjà esquissées** : Distressor EL8, API 2500 et Chandler Zener. Elles sont vectorielles, non photographiques et restent à comparer à l'exemplaire réel.
- **Toutes les autres entrées utilisent une façade générique clairement marquée NON VÉRIFIÉE**, avec 8 potentiomètres et 6 commutateurs de substitution. Ces réglages ne correspondent PAS aux réglages réels de chaque appareil.
- État audio inchangé (plugin de rappel, aucune DSP).

## Compilation Windows
Pré-requis : Visual Studio 2022 avec outils C++/MSVC, CMake 3.22+, Git, Internet pour télécharger JUCE 8.0.10.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target HardwareRecall_VST3
```

Le plugin est normalement généré dans `build/HardwareRecall_artefacts/Release/VST3/`. Copier le dossier `.vst3` dans `C:\Program Files\Common Files\VST3` et rescanner REAPER.

**Statut** : code source fourni ; pas de binaire Windows compilé ni de validation en hôte dans cet environnement.

## Notes de conception
La fidélité graphique et l'exactitude des positions exigent des photographies ou documents correspondant aux révisions exactes de tes unités. La bibliothèque exhaustive ne doit pas être confondue avec 203 façades déjà reproduites. Les modèles à façade non vérifiée restent des espaces de travail provisoires. L'ancienne v0.2 se recharge (le projet sauvegardé ne conserve que les appareils présents lors de l'enregistrement).

Le fichier `inventory.json` est un export lisible et `Source/Inventory.h` contient la liste embarquée à la compilation.
