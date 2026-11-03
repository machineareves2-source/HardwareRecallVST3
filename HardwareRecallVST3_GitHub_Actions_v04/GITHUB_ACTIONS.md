# Compiler Hardware Recall sur GitHub Actions

1. Créer un **nouveau dépôt GitHub**, par exemple `HardwareRecallVST3` (ne pas utiliser un dépôt consacré à un autre projet).
2. Déposer **les fichiers et dossiers contenus dans ce projet** à la racine du dépôt : `CMakeLists.txt`, `Source`, `inventory.json`, `.github/workflows/build-windows-vst3.yml`, etc. Attention : ne pas placer `HardwareRecallVST3` comme sous-dossier de la racine GitHub.
3. Sur GitHub, ouvrir l’onglet **Actions**, sélectionner **Build Hardware Recall VST3 (Windows)**, puis **Run workflow** (si disponible). Le workflow se lance aussi automatiquement à chaque push sur `main` ou `master`.
4. Ouvrir le run terminé et télécharger l’artifact **HardwareRecall-Windows-VST3**. Décompresser deux fois si GitHub a ajouté une archive autour du ZIP généré.
5. Copier le dossier `Hardware Recall.vst3` dans `C:\Program Files\Common Files\VST3\`, puis rescanner les VST3 dans REAPER.

⚠️ Le code C++ n'a pas été validé par une compilation Windows. Le workflow est prêt à lancer, mais son résultat peut être une erreur de build qu'il faudra corriger à partir des logs Actions.

Les machines à façade générique demeurent non vérifiées : ce workflow ne change pas la fidélité des interfaces.
