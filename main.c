#include <jansson.h>
#include <stdio.h>
#include <string.h>

// Fonction pour afficher les médicaments sous forme de tableau
void afficherMedicaments(json_t *tableau, const char *critere, const char *valeur) {
    size_t index;
    json_t *element;
    int trouve = 0;

    printf("\nMédicaments trouvés :\n");
    printf("| %-15s | %-15s | %-25s |\n", "Nom", "Marque", "Effet");
    printf("|-----------------|-----------------|-------------------------|\n");

    json_array_foreach(tableau, index, element) {
        if (strcmp(critere, "all") == 0 || strcmp(critere, "ALL") == 0) {
            // Afficher tous les médicaments
            const char *nom = json_string_value(json_object_get(element, "nom"));
            const char *marque = json_string_value(json_object_get(element, "marque"));
            const char *effet = json_string_value(json_object_get(element, "effet"));

            printf("| %-15s | %-15s | %-25s |\n", nom, marque, effet);

            trouve = 1;
        } else {
            json_t *critere_json = json_object_get(element, critere);
            const char *valeur_critere = json_string_value(critere_json);

            if (valeur_critere && strcmp(valeur_critere, valeur) == 0) {
                // Afficher le médicament correspondant au critère spécifié
                const char *nom = json_string_value(json_object_get(element, "nom"));
                const char *marque = json_string_value(json_object_get(element, "marque"));
                const char *effet = json_string_value(json_object_get(element, "effet"));

                printf("| %-15s | %-15s | %-25s |\n", nom, marque, effet);

                trouve = 1;
            }
        }
    }

    if (!trouve) {
        if (strcmp(critere, "all") == 0 || strcmp(critere, "ALL") == 0) {
            printf("Aucun médicament trouvé.\n");
        } else {
            printf("Aucun médicament trouvé avec le critère '%s=%s'.\n", critere, valeur);
        }
    }

    printf("\n");
}

int main() {
    // Ouvrir le fichier JSON en lecture
    FILE *fichier = fopen("medicaments.json", "r");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier JSON.\n");
        return 1;
    }

    // Charger le contenu du fichier dans un tableau JSON
    json_t *tableau;
    json_error_t erreur;
    tableau = json_loadf(fichier, 0, &erreur);
    fclose(fichier);

    // Vérifier les erreurs de chargement
    if (!tableau || !json_is_array(tableau)) {
        fprintf(stderr, "Erreur lors du chargement JSON : %s\n", erreur.text);
        return 1;
    }

    // Demander à l'utilisateur d'entrer le critère de recherche et la valeur
    char critere[20], valeur[50];
    printf("Entrez le critère de recherche (marque, effet, nom, all) : ");
    fgets(critere, sizeof(critere), stdin);
    critere[strcspn(critere, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne

    if (strcmp(critere, "all") != 0 && strcmp(critere, "ALL") != 0) {
        printf("Entrez la valeur du critère : ");
        fgets(valeur, sizeof(valeur), stdin);
        valeur[strcspn(valeur, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne
    }

    // Afficher les médicaments en fonction du critère spécifié
    afficherMedicaments(tableau, critere, valeur);

    // Libérer la mémoire allouée par Jansson
    json_decref(tableau);

    return 0;
}
