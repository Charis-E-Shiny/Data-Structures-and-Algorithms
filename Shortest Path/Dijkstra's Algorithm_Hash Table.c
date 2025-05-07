#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_STATIONS 100
#define INF INT_MAX

typedef struct {
    char station[50];
    int distance;
} Edge;

typedef struct {
    char name[50];
    int edges_count;
    Edge edges[MAX_STATIONS];
} Station;

Station graph[MAX_STATIONS];
int station_count = 0;

int stationIndex(char *station_name) {
    for (int i = 0; i < station_count; i++) {
        if (strcmp(graph[i].name, station_name) == 0) {
            return i;
        }
    }
    return -1;
}

void addStation(char *station_name) {
    if (stationIndex(station_name) == -1) {
        strcpy(graph[station_count].name, station_name);
        graph[station_count].edges_count = 0;
        station_count++;
    } else {
        printf("Station %s already exists.\n", station_name);
    }
}

void addEdge(char *from_station, char *to_station, int distance) {
    int from_idx = stationIndex(from_station);
    int to_idx = stationIndex(to_station);
    
    if (from_idx == -1 || to_idx == -1) {
        printf("One or both stations do not exist.\n");
        return;
    }

    graph[from_idx].edges[graph[from_idx].edges_count].distance = distance;
    strcpy(graph[from_idx].edges[graph[from_idx].edges_count].station, to_station);
    graph[from_idx].edges_count++;
}

void dijkstra(char *start_station) {
    int dist[MAX_STATIONS];
    int visited[MAX_STATIONS];
    int prev[MAX_STATIONS];
    for (int i = 0; i < station_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }

    int start_idx = stationIndex(start_station);
    dist[start_idx] = 0;

    for (int i = 0; i < station_count; i++) {
        int u = -1;
        for (int j = 0; j < station_count; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INF) break;

        visited[u] = 1;

        for (int v = 0; v < graph[u].edges_count; v++) {
            int adj_idx = stationIndex(graph[u].edges[v].station);
            int alt = dist[u] + graph[u].edges[v].distance;
            if (alt < dist[adj_idx]) {
                dist[adj_idx] = alt;
                prev[adj_idx] = u;
            }
        }
    }

    // Print the shortest path for each station
    for (int i = 0; i < station_count; i++) {
        if (dist[i] < INF) {
            printf("Distance from %s to %s: %d\n", start_station, graph[i].name, dist[i]);
        } else {
            printf("No path from %s to %s\n", start_station, graph[i].name);
        }
    }
}

void printMenu() {
    printf("\nMenu:\n");
    printf("1. Add Station\n");
    printf("2. Add Edge\n");
    printf("3. Run Dijkstra's Algorithm\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    char station1[50], station2[50];
    int distance;

    while (1) {
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume newline character after integer input

        switch (choice) {
            case 1:
                printf("Enter station name: ");
                fgets(station1, sizeof(station1), stdin);
                station1[strcspn(station1, "\n")] = '\0';  // Remove newline
                addStation(station1);
                break;
            
            case 2:
                printf("Enter first station: ");
                fgets(station1, sizeof(station1), stdin);
                station1[strcspn(station1, "\n")] = '\0';  // Remove newline
                
                printf("Enter second station: ");
                fgets(station2, sizeof(station2), stdin);
                station2[strcspn(station2, "\n")] = '\0';  // Remove newline

                printf("Enter distance: ");
                scanf("%d", &distance);
                getchar();  // Consume newline character
                addEdge(station1, station2, distance);
                break;
            
            case 3:
                printf("Enter starting station: ");
                fgets(station1, sizeof(station1), stdin);
                station1[strcspn(station1, "\n")] = '\0';  // Remove newline
                dijkstra(station1);
                break;
            
            case 4:
                printf("Exiting program.\n");
                return 0;
            
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
