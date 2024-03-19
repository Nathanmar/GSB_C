#include <jansson.h>
#include <stdio.h>
#include <string.h>

// Fonction pour afficher les médicaments sous forme de tableau
void afficherMedicaments(json_t *tableau, const char *critere, const char *valeur) {
    size_t index;
    json_t *element;
    int trouve = 0;

    printf("\nMédicaments trouvés :\n");
    printf("| %-15s | %-15s | %-25s | %-5s |\n", "Nom", "Marque", "Effet", "Stock");
    printf("|-----------------|-----------------|-------------------------|-------|\n");

    json_array_foreach(tableau, index, element) {
        if (strcmp(critere, "all") == 0 || strcmp(critere, "ALL") == 0) {
            // Afficher tous les médicaments
            const char *nom = json_string_value(json_object_get(element, "nom"));
            const char *marque = json_string_value(json_object_get(element, "marque"));
            const char *effet = json_string_value(json_object_get(element, "effet"));
            int stock = json_integer_value(json_object_get(element, "stock"));

            printf("| %-15s | %-15s | %-25s | %-5d |\n", nom, marque, effet, stock);

            trouve = 1;
        } else {
            json_t *critere_json = json_object_get(element, critere);
            const char *valeur_critere = json_string_value(critere_json);

            if (valeur_critere && strcmp(valeur_critere, valeur) == 0) {
                // Afficher le médicament correspondant au critère spécifié
                const char *nom = json_string_value(json_object_get(element, "nom"));
                const char *marque = json_string_value(json_object_get(element, "marque"));
                const char *effet = json_string_value(json_object_get(element, "effet"));
                int stock = json_integer_value(json_object_get(element, "stock"));

                printf("| %-15s | %-15s | %-25s | %-5d |\n", nom, marque, effet, stock);

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

    // Demander à l'utilisateur de choisir un médicament spécifique
    if (strcmp(critere, "all") != 0 && strcmp(critere, "ALL") != 0) {
        char choixMedicament[50];
        printf("Quel médicament spécifique choisissez-vous (entrez le nom exact) ? ");
        fgets(choixMedicament, sizeof(choixMedicament), stdin);
        choixMedicament[strcspn(choixMedicament, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne

        // Rechercher et afficher le médicament spécifique
        afficherMedicaments(tableau, "nom", choixMedicament);

        // Demander à l'utilisateur ce qu'il souhaite faire avec le stock
        int action;
        printf("Que voulez-vous faire avec le stock de ce médicament ?\n");
        printf("1. Ajouter du stock\n");
        printf("2. Retirer du stock\n");
        printf("Choix : ");
        scanf("%d", &action);

        json_t *medicament = NULL;
        json_t *stock = NULL;
        int quantite;
        int found = 0;
        switch (action) {
            case 1:
            printf("Entrez la quantité à ajouter du stock : ");
            scanf("%d", &quantite);
            for (size_t i = 0; i < json_array_size(tableau); i++) {
                json_t *element = json_array_get(tableau, i);
                const char *nom = json_string_value(json_object_get(element, "nom"));
                if (strcmp(nom, choixMedicament) == 0) {
                    // Ajouter la quantité de stock au médicament sélectionné
                    json_t *stock = json_object_get(element, "stock");
                    int stockActuel = json_integer_value(stock);
                    json_integer_set(stock, stockActuel + quantite);
                    printf("La quantité de stock pour %s a été mise à jour.\n", nom);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Le médicament spécifié n'a pas été trouvé.\n");
            }
                break;
            case 2:
                printf("Entrez la quantité à retirer du stock : ");
            scanf("%d", &quantite);
            for (size_t i = 0; i < json_array_size(tableau); i++) {
                json_t *element = json_array_get(tableau, i);
                const char *nom = json_string_value(json_object_get(element, "nom"));
                if (strcmp(nom, choixMedicament) == 0) {
                    // Ajouter la quantité de stock au médicament sélectionné
                    json_t *stock = json_object_get(element, "stock");
                    int stockActuel = json_integer_value(stock);
                    json_integer_set(stock, stockActuel - quantite);
                    printf("La quantité de stock pour %s a été mise à jour.\n", nom);
                    found = 1;
                    break;
                }
            }
            default:
                printf("Choix invalide.\n");
                break;
        }
        FILE *fichier_modifie = fopen("medicaments.json", "w");
        if (!fichier_modifie) {
            fprintf(stderr, "Impossible d'ouvrir le fichier JSON pour écriture.\n");
            return 1;
        }
        json_dumpf(tableau, fichier_modifie, JSON_INDENT(2));
        fclose(fichier_modifie);
        afficherMedicaments(tableau, "nom", choixMedicament);
    }

    // Libérer la mémoire allouée par Jansson
    json_decref(tableau);

    return 0;
}
