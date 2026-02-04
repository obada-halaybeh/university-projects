#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define HASH_TABLE_SIZE 101
#define AUDIT_LOG_FILE "audit_log.csv"

//company.
struct Company {
    int  companyID;
    char companyName[101];
    char phone[21];
    char email[101];
};

struct CompanyNode {
    struct Company data;
    struct CompanyNode *next;
    struct CompanyNode *prev;
};

struct CompanyTable {
    struct CompanyNode *head;
    struct CompanyNode *tail;
    int count;
};

// Access role
struct Role {
    int  roleID;
    char roleName[51];
    char description[256];
};

struct RoleNode {
    struct Role data;
    struct RoleNode *next;
    struct RoleNode *prev;
};

struct RoleTable {
    struct RoleNode *head;
    struct RoleNode *tail;
    int count;
};

// Users
struct User {
    int  userID;
    char fullName[101];
    char ssn[21];
    char email[101];
    char phone[21];
    int  roleID;
    int  companyID;
};

struct UserNode {
    struct User data;
    struct UserNode *next;
    struct UserNode *prev;
};

struct UserTable {
    struct UserNode *head;
    struct UserNode *tail;
    int count;
};

// Vehicles
struct Vehicle {
    int  vehicleID;
    char licensePlate[21];
    char make[51];
    char model[51];
    char color[31];
    char vehicleType[31];
    int  userID;
};

struct VehicleNode {
    struct Vehicle data;
    struct VehicleNode *next;
    struct VehicleNode *prev;
};

struct VehicleTable {
    struct VehicleNode *head;
    struct VehicleNode *tail;
    int count;
};

// Gate
struct Gate {
    int  gateID;
    char gateName[51];
    char gateType[31];
};

struct GateNode {
    struct Gate data;
    struct GateNode *next;
    struct GateNode *prev;
};

struct GateTable {
    struct GateNode *head;
    struct GateNode *tail;
    int count;
};

// Access schedule
struct AccessSchedule {
    int  scheduleID;
    char scheduleName[51];
    char daysOfWeek[21];
};

struct AccessScheduleNode {
    struct AccessSchedule data;
    struct AccessScheduleNode *next;
    struct AccessScheduleNode *prev;
};

struct AccessScheduleTable {
    struct AccessScheduleNode *head;
    struct AccessScheduleNode *tail;
    int count;
};

// Access rule
struct AccessRule {
    int  accessRuleID;
    int  isActive;
    int  priority;
    int  roleID;
    int  gateID;
    int  scheduleID;
};

struct AccessRuleNode {
    struct AccessRule data;
    struct AccessRuleNode *next;
    struct AccessRuleNode *prev;
};

struct AccessRuleTable {
    struct AccessRuleNode *head;
    struct AccessRuleNode *tail;
    int count;
};

// Access log
struct AccessLog {
    int  accessLogID;
    char accessTime[20];
    char result[11];
    int  vehicleID;
    int  gateID;
};

struct AccessLogNode {
    struct AccessLog data;
    struct AccessLogNode *next;
    struct AccessLogNode *prev;
};

struct AccessLogTable {
    struct AccessLogNode *head;
    struct AccessLogNode *tail;
    int count;
};


// loging
static void getCurrentTimestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}


struct AuditLog {
    int logID;
    int recordID;
    char tableName[50];
    char columnName[50];
    char oldValue[120];
    char newValue[120];
    char actionType[20];
    char timestamp[25];
};


static int getNextAuditLogID(const char *filename){
    FILE *fp = fopen(filename,"r");
    if(!fp) return 1;

    char line[512];
    int maxID = 0;


    if(fgets(line,sizeof(line), fp) == NULL){
        fclose(fp);
        return 1;
    }
    while(fgets(line,sizeof(line), fp) != NULL){
        int id = 0;
        if(sscanf(line, "%d", &id) == 1){
            if(id > maxID) maxID = id;
        }
    }
    fclose(fp);
    return maxID + 1;

}


static void ensureAuditLogHeader(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        return;
    }

    fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "logID,recordID,tableName,columnName,oldValue,newValue,actionType,timestamp\n");
    fclose(fp);
}


static void writeAuditLog(const char *filename,
                          int recordID,
                          const char *tableName,
                          const char *columnName,
                          const char *oldValue,
                          const char *newValue,
                          const char *actionType){



    ensureAuditLogHeader(filename);

    FILE *fp = fopen(filename, "a");
    if(!fp){
        printf("Could not open audit log file.\n");
        return;
    }

    int id = getNextAuditLogID(filename);
    if (!oldValue || oldValue[0] == '\0') oldValue = "-";
    if (!newValue || newValue[0] == '\0') newValue = "-";
    if (!columnName || columnName[0] == '\0') columnName = "-";

    char ts[25];
    getCurrentTimestamp(ts, sizeof(ts));

    fprintf(fp, "%d,%d,%s,%s,%s,%s,%s,%s\n",
            id,
            recordID,
            tableName,
            columnName,
            oldValue,
            newValue,
            actionType,
            ts);

    fclose(fp);
}


void printAuditLog(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No audit log file found.\n");
        return;
    }

    char line[1024];


    fgets(line, sizeof(line), fp);

    printf("\n================ AUDIT LOG ================\n");
    printf("%-5s %-8s %-15s %-15s %-15s %-15s %-10s %-20s\n",
           "ID", "REC_ID", "TABLE", "COLUMN", "OLD", "NEW", "ACTION", "TIME");
    printf("-------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        struct AuditLog log;

        int n = sscanf(line,
            "%d,%d,%49[^,],%49[^,],%119[^,],%119[^,],%19[^,],%24[^\n]",
            &log.logID,
            &log.recordID,
            log.tableName,
            log.columnName,
            log.oldValue,
            log.newValue,
            log.actionType,
            log.timestamp
        );

        if (n == 8) {
            printf("%-5d %-8d %-15s %-15s %-15s %-15s %-10s %-20s\n",
                   log.logID, log.recordID, log.tableName, log.columnName,
                   log.oldValue, log.newValue, log.actionType, log.timestamp);
        }
    }

    fclose(fp);
}






// indexing
static struct CompanyNode* companyHash[HASH_TABLE_SIZE];

static struct CompanyNode companyDummyNode;
static struct CompanyNode* COMPANY_DUMMY = &companyDummyNode;

static void companyHashInit(void){
    for(int i=0; i< HASH_TABLE_SIZE; i++){
        companyHash[i] = NULL;
    }

}

static int companyHashFunc(int companyID) {
    return companyID % HASH_TABLE_SIZE;
}

static int companyHashInsertNode(struct CompanyNode* node){
    if(node == NULL) return 0;

    int key = node->data.companyID;
    int idx = companyHashFunc(key);
    int start = idx;

    while(companyHash[idx] != NULL && companyHash[idx] != COMPANY_DUMMY){


        if(companyHash[idx]->data.companyID == key) return 0;

        idx = (idx + 1) % HASH_TABLE_SIZE;


        if(idx == start) return 0;
    }

    companyHash[idx] = node;
    return 1;

}

static struct CompanyNode* companyHashFindNode(int companyID){
    int idx = companyHashFunc(companyID);
    int start = idx;

    while(companyHash[idx] != NULL){

        if(companyHash[idx] != COMPANY_DUMMY && companyHash[idx]->data.companyID == companyID){
            return companyHash[idx];
        }
        idx = (idx+1)% HASH_TABLE_SIZE;
        if(idx == start) break;
    }

    return NULL;
}

static int companyHashRemoveNode(int companyID){
    int idx = companyHashFunc(companyID);
    int start = idx;

    while(companyHash[idx] !=NULL){
        if(companyHash[idx] != COMPANY_DUMMY && companyHash[idx]->data.companyID == companyID){
            companyHash[idx] = COMPANY_DUMMY;
            return 1;
        }
        idx = (idx+1)% HASH_TABLE_SIZE;
        if(idx == start) break;
    }
    return 0;
}

static void companyHashBuildFromList(struct CompanyTable* t){
    companyHashInit();
    struct CompanyNode* cur = t->head;
    while(cur != NULL){
        companyHashInsertNode(cur);
        cur = cur->next;
    }
}


// Role hash index.
static struct RoleNode* roleHash[HASH_TABLE_SIZE];
static struct RoleNode roleDummyNode;
static struct RoleNode* ROLE_DUMMY = &roleDummyNode;

static void roleHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) roleHash[i]=NULL;
}

static int roleHashFunc(int roleID){
    return roleID % HASH_TABLE_SIZE;
}

static int roleHashInsertNode(struct RoleNode* node){
    if(node==NULL) return 0;
    int key=node->data.roleID;
    int idx=roleHashFunc(key), start=idx;

    while(roleHash[idx]!=NULL && roleHash[idx]!=ROLE_DUMMY){
        if(roleHash[idx]->data.roleID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    roleHash[idx]=node;
    return 1;
}

static struct RoleNode* roleHashFindNode(int roleID){
    int idx=roleHashFunc(roleID), start=idx;

    while(roleHash[idx]!=NULL){
        if(roleHash[idx]!=ROLE_DUMMY && roleHash[idx]->data.roleID==roleID)
            return roleHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int roleHashRemoveNode(int roleID){
    int idx=roleHashFunc(roleID), start=idx;

    while(roleHash[idx]!=NULL){
        if(roleHash[idx]!=ROLE_DUMMY && roleHash[idx]->data.roleID==roleID){
            roleHash[idx]=ROLE_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void roleHashBuildFromList(struct RoleTable* t){
    roleHashInit();
    struct RoleNode* cur=t->head;
    while(cur!=NULL){
        roleHashInsertNode(cur);
        cur=cur->next;
    }
}


// User hash index.
static struct UserNode* userHash[HASH_TABLE_SIZE];
static struct UserNode userDummyNode;
static struct UserNode* USER_DUMMY = &userDummyNode;

static void userHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) userHash[i]=NULL;
}

static int userHashFunc(int userID){
    return userID % HASH_TABLE_SIZE;
}

static int userHashInsertNode(struct UserNode* node){
    if(node==NULL) return 0;
    int key=node->data.userID;
    int idx=userHashFunc(key), start=idx;

    while(userHash[idx]!=NULL && userHash[idx]!=USER_DUMMY){
        if(userHash[idx]->data.userID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    userHash[idx]=node;
    return 1;
}

static struct UserNode* userHashFindNode(int userID){
    int idx=userHashFunc(userID), start=idx;

    while(userHash[idx]!=NULL){
        if(userHash[idx]!=USER_DUMMY && userHash[idx]->data.userID==userID)
            return userHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int userHashRemoveNode(int userID){
    int idx=userHashFunc(userID), start=idx;

    while(userHash[idx]!=NULL){
        if(userHash[idx]!=USER_DUMMY && userHash[idx]->data.userID==userID){
            userHash[idx]=USER_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void userHashBuildFromList(struct UserTable* t){
    userHashInit();
    struct UserNode* cur=t->head;
    while(cur!=NULL){
        userHashInsertNode(cur);
        cur=cur->next;
    }
}



// Vehicle hash index.
static struct VehicleNode* vehicleHash[HASH_TABLE_SIZE];
static struct VehicleNode vehicleDummyNode;
static struct VehicleNode* VEHICLE_DUMMY = &vehicleDummyNode;

static void vehicleHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) vehicleHash[i]=NULL;
}

static int vehicleHashFunc(int vehicleID){
    return vehicleID % HASH_TABLE_SIZE;
}

static int vehicleHashInsertNode(struct VehicleNode* node){
    if(node==NULL) return 0;
    int key=node->data.vehicleID;
    int idx=vehicleHashFunc(key), start=idx;

    while(vehicleHash[idx]!=NULL && vehicleHash[idx]!=VEHICLE_DUMMY){
        if(vehicleHash[idx]->data.vehicleID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    vehicleHash[idx]=node;
    return 1;
}

static struct VehicleNode* vehicleHashFindNode(int vehicleID){
    int idx=vehicleHashFunc(vehicleID), start=idx;

    while(vehicleHash[idx]!=NULL){
        if(vehicleHash[idx]!=VEHICLE_DUMMY && vehicleHash[idx]->data.vehicleID==vehicleID)
            return vehicleHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int vehicleHashRemoveNode(int vehicleID){
    int idx=vehicleHashFunc(vehicleID), start=idx;

    while(vehicleHash[idx]!=NULL){
        if(vehicleHash[idx]!=VEHICLE_DUMMY && vehicleHash[idx]->data.vehicleID==vehicleID){
            vehicleHash[idx]=VEHICLE_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void vehicleHashBuildFromList(struct VehicleTable* t){
    vehicleHashInit();
    struct VehicleNode* cur=t->head;
    while(cur!=NULL){
        vehicleHashInsertNode(cur);
        cur=cur->next;
    }
}


// Gate hash index.
static struct GateNode* gateHash[HASH_TABLE_SIZE];
static struct GateNode gateDummyNode;
static struct GateNode* GATE_DUMMY = &gateDummyNode;

static void gateHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) gateHash[i]=NULL;
}

static int gateHashFunc(int gateID){
    return gateID % HASH_TABLE_SIZE;
}

static int gateHashInsertNode(struct GateNode* node){
    if(node==NULL) return 0;
    int key=node->data.gateID;
    int idx=gateHashFunc(key), start=idx;

    while(gateHash[idx]!=NULL && gateHash[idx]!=GATE_DUMMY){
        if(gateHash[idx]->data.gateID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    gateHash[idx]=node;
    return 1;
}

static struct GateNode* gateHashFindNode(int gateID){
    int idx=gateHashFunc(gateID), start=idx;

    while(gateHash[idx]!=NULL){
        if(gateHash[idx]!=GATE_DUMMY && gateHash[idx]->data.gateID==gateID)
            return gateHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int gateHashRemoveNode(int gateID){
    int idx=gateHashFunc(gateID), start=idx;

    while(gateHash[idx]!=NULL){
        if(gateHash[idx]!=GATE_DUMMY && gateHash[idx]->data.gateID==gateID){
            gateHash[idx]=GATE_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void gateHashBuildFromList(struct GateTable* t){
    gateHashInit();
    struct GateNode* cur=t->head;
    while(cur!=NULL){
        gateHashInsertNode(cur);
        cur=cur->next;
    }
}


// Schedule hash index.
static struct AccessScheduleNode* scheduleHash[HASH_TABLE_SIZE];
static struct AccessScheduleNode scheduleDummyNode;
static struct AccessScheduleNode* SCHEDULE_DUMMY = &scheduleDummyNode;

static void scheduleHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) scheduleHash[i]=NULL;
}

static int scheduleHashFunc(int scheduleID){
    return scheduleID % HASH_TABLE_SIZE;
}

static int scheduleHashInsertNode(struct AccessScheduleNode* node){
    if(node==NULL) return 0;
    int key=node->data.scheduleID;
    int idx=scheduleHashFunc(key), start=idx;

    while(scheduleHash[idx]!=NULL && scheduleHash[idx]!=SCHEDULE_DUMMY){
        if(scheduleHash[idx]->data.scheduleID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    scheduleHash[idx]=node;
    return 1;
}

static struct AccessScheduleNode* scheduleHashFindNode(int scheduleID){
    int idx=scheduleHashFunc(scheduleID), start=idx;

    while(scheduleHash[idx]!=NULL){
        if(scheduleHash[idx]!=SCHEDULE_DUMMY && scheduleHash[idx]->data.scheduleID==scheduleID)
            return scheduleHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int scheduleHashRemoveNode(int scheduleID){
    int idx=scheduleHashFunc(scheduleID), start=idx;

    while(scheduleHash[idx]!=NULL){
        if(scheduleHash[idx]!=SCHEDULE_DUMMY && scheduleHash[idx]->data.scheduleID==scheduleID){
            scheduleHash[idx]=SCHEDULE_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void scheduleHashBuildFromList(struct AccessScheduleTable* t){
    scheduleHashInit();
    struct AccessScheduleNode* cur=t->head;
    while(cur!=NULL){
        scheduleHashInsertNode(cur);
        cur=cur->next;
    }
}



// Access rule hash index.
static struct AccessRuleNode* accessRuleHash[HASH_TABLE_SIZE];
static struct AccessRuleNode accessRuleDummyNode;
static struct AccessRuleNode* ACCESSRULE_DUMMY = &accessRuleDummyNode;

static void accessRuleHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) accessRuleHash[i]=NULL;
}

static int accessRuleHashFunc(int accessRuleID){
    return accessRuleID % HASH_TABLE_SIZE;
}

static int accessRuleHashInsertNode(struct AccessRuleNode* node){
    if(node==NULL) return 0;
    int key=node->data.accessRuleID;
    int idx=accessRuleHashFunc(key), start=idx;

    while(accessRuleHash[idx]!=NULL && accessRuleHash[idx]!=ACCESSRULE_DUMMY){
        if(accessRuleHash[idx]->data.accessRuleID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    accessRuleHash[idx]=node;
    return 1;
}

static struct AccessRuleNode* accessRuleHashFindNode(int accessRuleID){
    int idx=accessRuleHashFunc(accessRuleID), start=idx;

    while(accessRuleHash[idx]!=NULL){
        if(accessRuleHash[idx]!=ACCESSRULE_DUMMY &&
           accessRuleHash[idx]->data.accessRuleID==accessRuleID)
            return accessRuleHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int accessRuleHashRemoveNode(int accessRuleID){
    int idx=accessRuleHashFunc(accessRuleID), start=idx;

    while(accessRuleHash[idx]!=NULL){
        if(accessRuleHash[idx]!=ACCESSRULE_DUMMY &&
           accessRuleHash[idx]->data.accessRuleID==accessRuleID){
            accessRuleHash[idx]=ACCESSRULE_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void accessRuleHashBuildFromList(struct AccessRuleTable* t){
    accessRuleHashInit();
    struct AccessRuleNode* cur=t->head;
    while(cur!=NULL){
        accessRuleHashInsertNode(cur);
        cur=cur->next;
    }
}



// Access log hash index.
static struct AccessLogNode* accessLogHash[HASH_TABLE_SIZE];
static struct AccessLogNode accessLogDummyNode;
static struct AccessLogNode* ACCESSLOG_DUMMY = &accessLogDummyNode;

static void accessLogHashInit(void){
    for(int i=0;i<HASH_TABLE_SIZE;i++) accessLogHash[i]=NULL;
}

static int accessLogHashFunc(int accessLogID){
    return accessLogID % HASH_TABLE_SIZE;
}

static int accessLogHashInsertNode(struct AccessLogNode* node){
    if(node==NULL) return 0;
    int key=node->data.accessLogID;
    int idx=accessLogHashFunc(key), start=idx;

    while(accessLogHash[idx]!=NULL && accessLogHash[idx]!=ACCESSLOG_DUMMY){
        if(accessLogHash[idx]->data.accessLogID==key) return 0;
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) return 0;
    }
    accessLogHash[idx]=node;
    return 1;
}

static struct AccessLogNode* accessLogHashFindNode(int accessLogID){
    int idx=accessLogHashFunc(accessLogID), start=idx;

    while(accessLogHash[idx]!=NULL){
        if(accessLogHash[idx]!=ACCESSLOG_DUMMY &&
           accessLogHash[idx]->data.accessLogID==accessLogID)
            return accessLogHash[idx];
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return NULL;
}

static int accessLogHashRemoveNode(int accessLogID){
    int idx=accessLogHashFunc(accessLogID), start=idx;

    while(accessLogHash[idx]!=NULL){
        if(accessLogHash[idx]!=ACCESSLOG_DUMMY &&
           accessLogHash[idx]->data.accessLogID==accessLogID){
            accessLogHash[idx]=ACCESSLOG_DUMMY;
            return 1;
        }
        idx=(idx+1)%HASH_TABLE_SIZE;
        if(idx==start) break;
    }
    return 0;
}

static void accessLogHashBuildFromList(struct AccessLogTable* t){
    accessLogHashInit();
    struct AccessLogNode* cur=t->head;
    while(cur!=NULL){
        accessLogHashInsertNode(cur);
        cur=cur->next;
    }
}



// Simple file existence check for CSV creation.
int fileExists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}



// Create CSV headers for each table if missing.
void createCompanyCSVHeader(const char *filename){

if(fileExists(filename)) return;

 FILE *fp = fopen(filename, "w");
 if(!fp){
    printf("Error: could not create %s\n", filename);
    return;
 }

 fprintf(fp, "companyID,companyName,phone,email\n");
 fclose(fp);

 printf("%s created with header.\n", filename);

}

void createRoleCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "roleID,roleName,description\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createUserCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "userID,fullName,ssn,email,phone,roleID,companyID\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createVehicleCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "vehicleID,licensePlate,make,model,color,vehicleType,userID\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createGateCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "gateID,gateName,gateType\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createAccessScheduleCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "scheduleID,scheduleName,daysOfWeek\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createAccessRuleCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "accessRuleID,isActive,priority,roleID,gateID,scheduleID\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}

void createAccessLogCSVHeader(const char *filename) {
    if (fileExists(filename)) return;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not create %s\n", filename);
        return;
    }

    fprintf(fp, "accessLogID,accessTime,result,vehicleID,gateID\n");

    fclose(fp);
    printf("%s created with header.\n", filename);
}



// Company validation helpers and CSV I/O.
int companyIDExists(struct CompanyTable *table, int companyID) {

    (void)table;
    return companyHashFindNode(companyID) != NULL;


}

int companyEmailExists(struct CompanyTable *table, const char *email) {

    struct CompanyNode *curr = table->head;
    while (curr != NULL) {
        if (strcmp(curr->data.email, email) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

int validateCompany(struct CompanyTable *table, struct Company c) {


    if (c.companyID <= 0) {
        printf("Constraint error: companyID must be > 0\n");
        return 0;
    }


    if (companyIDExists(table, c.companyID)) {
        printf("Constraint error: companyID already exists\n");
        return 0;
    }


    if (strlen(c.companyName) == 0) {
        printf("Constraint error: companyName cannot be empty\n");
        return 0;
    }


    if (strlen(c.phone) == 0) {
        printf("Constraint error: phone cannot be empty\n");
        return 0;
    }


    if (strlen(c.email) == 0) {
        printf("Constraint error: email cannot be empty\n");
        return 0;
    }


    if (companyEmailExists(table, c.email)) {
        printf("Constraint error: email already exists\n");
        return 0;
    }

    return 1;
}

int getNextCompanyID(struct CompanyTable *t) {
    int maxID = 0;
    struct CompanyNode *curr = t->head;

    while (curr != NULL) {
        if (curr->data.companyID > maxID)
            maxID = curr->data.companyID;
        curr = curr->next;
    }

    return maxID + 1;
}

int isCompanyUsedByUser(struct UserTable *users, int companyID) {
    struct UserNode *u = users->head;
    while (u != NULL) {
        if (u->data.companyID == companyID) return 1;
        u = u->next;
    }
    return 0;
}






void addCompanyToTable(struct CompanyTable *t, struct Company c) {
    struct CompanyNode *node = malloc(sizeof(struct CompanyNode));

    node->data = c;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}
void loadCompanyTable(struct CompanyTable *table, const char *filename) {


    table->head = NULL;
    table->tail = NULL;
    table->count = 0;


    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return;
    }

    char line[256];


    fgets(line, sizeof(line), fp);


    while (fgets(line, sizeof(line), fp) != NULL) {
        struct Company c;

        int n = sscanf(line, "%d,%100[^,],%20[^,],%100[^\n]",
                       &c.companyID,
                       c.companyName,
                       c.phone,
                       c.email);

        if (n == 4) {
            addCompanyToTable(table, c);
        } else {
            printf("Skipping invalid line: %s", line);
        }
    }

    fclose(fp);
    companyHashBuildFromList(table);
}
void saveCompanyTableToCSV(struct CompanyTable *table, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Could not open %s\n", filename);
        return;
    }

    fprintf(fp, "companyID,companyName,phone,email\n");

    struct CompanyNode *curr = table->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s,%s\n",
                curr->data.companyID,
                curr->data.companyName,
                curr->data.phone,
                curr->data.email);
        curr = curr->next;
    }

    fclose(fp);
}
int addNewCompany(struct CompanyTable *t, struct Company c, const char *filename) {


    if (validateCompany(t, c) == 0) {
        return 0;
    }


    struct CompanyNode *node = malloc(sizeof(struct CompanyNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = c;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;


    companyHashInsertNode(node);

writeAuditLog(AUDIT_LOG_FILE, c.companyID, "Company", "companyName", "", c.companyName, "INSERT");
writeAuditLog(AUDIT_LOG_FILE, c.companyID, "Company", "phone",       "", c.phone,       "INSERT");
writeAuditLog(AUDIT_LOG_FILE, c.companyID, "Company", "email",       "", c.email,       "INSERT");



    saveCompanyTableToCSV(t, filename);

    return 1;
}
int deleteCompanyByID(struct CompanyTable *table,
                      struct UserTable *users,
                      int companyID) {


    if (isCompanyUsedByUser(users, companyID)) {
        printf("Delete blocked: companyID %d is referenced by a user.\n", companyID);
        return 0;
    }



    struct CompanyNode *curr = companyHashFindNode(companyID);
    if(curr == NULL){
        printf("Company not found.\n");
        return 0;
    }


            if (curr->prev == NULL)//head
                table->head = curr->next;
            else//other
                curr->prev->next = curr->next;


            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            writeAuditLog(AUDIT_LOG_FILE, companyID, "Company", "*", "", "-", "DELETE");

            companyHashRemoveNode(companyID);
            free(curr);
            table->count--;

            return 1;

}
int updateCompany(struct CompanyTable *t,
                  int companyID,
                  struct Company updated,
                  const char *filename) {

    struct CompanyNode *target = companyHashFindNode(companyID);

    if (target == NULL) {
        printf("Company not found.\n");
        return 0;
    }


    if (strlen(updated.companyName) == 0) {
        printf("Constraint error: companyName cannot be empty\n");
        return 0;
    }
    if (strlen(updated.phone) == 0) {
        printf("Constraint error: phone cannot be empty\n");
        return 0;
    }
    if (strlen(updated.email) == 0) {
        printf("Constraint error: email cannot be empty\n");
        return 0;
    }


    struct CompanyNode *curr = t->head;
    while (curr != NULL) {
        if (curr->data.companyID != companyID &&
            strcmp(curr->data.email, updated.email) == 0) {
            printf("Constraint error: email already exists\n");
            return 0;
        }
        curr = curr->next;
    }


    if (strcmp(target->data.companyName, updated.companyName) != 0)
        writeAuditLog(AUDIT_LOG_FILE, companyID, "Company", "companyName",
                      target->data.companyName, updated.companyName, "UPDATE");

    if (strcmp(target->data.phone, updated.phone) != 0)
        writeAuditLog(AUDIT_LOG_FILE, companyID, "Company", "phone",
                      target->data.phone, updated.phone, "UPDATE");

    if (strcmp(target->data.email, updated.email) != 0)
        writeAuditLog(AUDIT_LOG_FILE, companyID, "Company", "email",
                      target->data.email, updated.email, "UPDATE");


    strcpy(target->data.companyName, updated.companyName);
    strcpy(target->data.phone, updated.phone);
    strcpy(target->data.email, updated.email);

    saveCompanyTableToCSV(t, filename);
    return 1;
}




// Role validation helpers and CSV I/O.
int roleIDExists(struct RoleTable *t, int roleID) {

    (void)t;
    return roleHashFindNode(roleID) != NULL;

}

int roleNameExists(struct RoleTable *t, const char *roleName) {
    if (roleName == NULL || strlen(roleName) == 0) return 0;

    struct RoleNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.roleName, roleName) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int validateRole(struct RoleTable *t, struct Role r) {


    if (r.roleID <= 0) {
        printf("Constraint error: roleID must be > 0\n");
        return 0;
    }


    if (roleIDExists(t, r.roleID)) {
        printf("Constraint error: roleID already exists\n");
        return 0;
    }


    if (strlen(r.roleName) == 0) {
        printf("Constraint error: roleName cannot be empty\n");
        return 0;
    }


    if (roleNameExists(t, r.roleName)) {
        printf("Constraint error: roleName already exists\n");
        return 0;
    }

    if (strlen(r.description) == 0) {
    printf("Constraint error: description cannot be empty\n");
    return 0;
    }


    return 1;
}

int getNextRoleID(struct RoleTable *table) {
    int maxID = 0;
    struct RoleNode *curr = table->head;

    while (curr != NULL) {
        if (curr->data.roleID > maxID) {
            maxID = curr->data.roleID;
        }
        curr = curr->next;
    }

    return maxID + 1;
}


int isRoleUsedByUser(struct UserTable *users, int roleID) {
    struct UserNode *u = users->head;
    while (u != NULL) {
        if (u->data.roleID == roleID) return 1;
        u = u->next;
    }
    return 0;
}

int isRoleUsedByAccessRule(struct AccessRuleTable *rules, int roleID) {
    struct AccessRuleNode *curr = rules->head;
    while (curr != NULL) {
        if (curr->data.roleID == roleID) return 1;
        curr = curr->next;
    }
    return 0;
}





void addRoleToTable(struct RoleTable *t, struct Role r) {
    struct RoleNode *node = malloc(sizeof(struct RoleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = r;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}
void loadRoleTable(struct RoleTable *table, const char *filename) {


    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return;
    }

    char line[400];


    fgets(line, sizeof(line), fp);


    while (fgets(line, sizeof(line), fp) != NULL) {
        struct Role r;



        int n = sscanf(line, "%d,%50[^,],%255[^\n]",
                       &r.roleID,
                       r.roleName,
                       r.description);


        if (n == 3) {
            addRoleToTable(table, r);
        } else {
            printf("Skipping invalid line: %s", line);
        }
    }

    fclose(fp);

    roleHashBuildFromList(table);
}
void saveRoleTableToCSV(struct RoleTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "roleID,roleName,description\n");

    struct RoleNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s\n",
                curr->data.roleID,
                curr->data.roleName,
                curr->data.description);
        curr = curr->next;
    }

    fclose(fp);
}
int addNewRole(struct RoleTable *t, struct Role r, const char *filename) {


    if (!validateRole(t, r)) {
        return 0;
    }


    struct RoleNode *node = malloc(sizeof(struct RoleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }


    node->data = r;
    node->next = NULL;
    node->prev = t->tail;


    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;


    roleHashInsertNode(node);

    writeAuditLog(AUDIT_LOG_FILE, r.roleID, "Role", "roleName",    "", r.roleName,    "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, r.roleID, "Role", "description", "", r.description, "INSERT");


    saveRoleTableToCSV(t, filename);

    return 1;
}

int deleteRoleByID(struct RoleTable *table,
                   struct UserTable *users,
                   struct AccessRuleTable *rules,
                   int roleID) {


    if (isRoleUsedByUser(users, roleID)) {
        printf("Delete blocked: roleID %d is referenced by Users.\n", roleID);
        return 0;
    }


    if (isRoleUsedByAccessRule(rules, roleID)) {
        printf("Delete blocked: roleID %d is referenced by AccessRules.\n", roleID);
        return 0;
    }

    struct RoleNode *curr = roleHashFindNode(roleID);
    if(curr == NULL){
    printf("Role not found.\n");
    return 0;
    }


        if (curr->data.roleID == roleID) {

            if (curr->prev == NULL)
                table->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;

            roleHashRemoveNode(roleID);

            writeAuditLog(AUDIT_LOG_FILE, roleID, "Role", "*", "", "-", "DELETE");

            free(curr);
            table->count--;

            return 1;
        }




}
int updateRole(struct RoleTable *t,
               int roleID,
               struct Role updated,
               const char *filename) {

    struct RoleNode *target = roleHashFindNode(roleID);

    if (target == NULL) {
        printf("Role not found.\n");
        return 0;
    }

    if (strlen(updated.roleName) == 0) {
        printf("Constraint error: roleName cannot be empty\n");
        return 0;
    }

    if (strlen(updated.description) == 0) {
        printf("Constraint error: description cannot be empty\n");
        return 0;
    }


    struct RoleNode *curr = t->head;
    while (curr != NULL) {
        if (curr->data.roleID != roleID &&
            strcmp(curr->data.roleName, updated.roleName) == 0) {
            printf("Constraint error: roleName already exists\n");
            return 0;
        }
        curr = curr->next;
    }


    if (strcmp(target->data.roleName, updated.roleName) != 0)
        writeAuditLog(AUDIT_LOG_FILE, roleID, "Role", "roleName",
                      target->data.roleName, updated.roleName, "UPDATE");

    if (strcmp(target->data.description, updated.description) != 0)
        writeAuditLog(AUDIT_LOG_FILE, roleID, "Role", "description",
                      target->data.description, updated.description, "UPDATE");



    strcpy(target->data.roleName, updated.roleName);
    strcpy(target->data.description, updated.description);

    saveRoleTableToCSV(t, filename);
    return 1;
}






int getNextUserID(struct UserTable *table) {
    int maxID = 0;
    struct UserNode *curr = table->head;

    while (curr != NULL) {
        if (curr->data.userID > maxID)
            maxID = curr->data.userID;
        curr = curr->next;
    }
    return maxID + 1;
}


// User validation helpers and CSV I/O.
int userIDExists(struct UserTable *t, int userID) {
    (void)t;
    return userHashFindNode(userID) != NULL;
}

int userSSNExists(struct UserTable *t, const char *ssn) {
    struct UserNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.ssn, ssn) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int userEmailExists(struct UserTable *t, const char *email) {
    struct UserNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.email, email) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int validateUser(struct UserTable *users,
                 struct RoleTable *roles,
                 struct CompanyTable *companies,
                 struct User u) {


    if (u.userID <= 0) {
        printf("Constraint error: userID must be > 0\n");
        return 0;
    }
    if (userIDExists(users, u.userID)) {
        printf("Constraint error: userID already exists\n");
        return 0;
    }


    if (strlen(u.fullName) == 0) {
        printf("Constraint error: fullName cannot be empty\n");
        return 0;
    }


    if (strlen(u.ssn) == 0) {
        printf("Constraint error: ssn cannot be empty\n");
        return 0;
    }


    if (strlen(u.email) == 0) {
        printf("Constraint error: email cannot be empty\n");
        return 0;
    }


    if (strlen(u.phone) == 0) {
        printf("Constraint error: phone cannot be empty\n");
        return 0;
    }


    if (userSSNExists(users, u.ssn)) {
        printf("Constraint error: ssn already exists\n");
        return 0;
    }
    if (userEmailExists(users, u.email)) {
        printf("Constraint error: email already exists\n");
        return 0;
    }


    if (!roleIDExists(roles, u.roleID)) {
        printf("Constraint error: roleID does not exist\n");
        return 0;
    }
    if (!companyIDExists(companies, u.companyID)) {
        printf("Constraint error: companyID does not exist\n");
        return 0;
    }

    return 1;
}

int isUserUsedByVehicle(struct VehicleTable *vehicles, int userID) {
    struct VehicleNode *v = vehicles->head;
    while (v != NULL) {
        if (v->data.userID == userID) {
            return 1;
        }
        v = v->next;
    }
    return 0;
}



void addUserToTable(struct UserTable *t, struct User u) {
    struct UserNode *node = malloc(sizeof(struct UserNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = u;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadUserTable(struct UserTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        userHashInit();
        return;
    }

    char line[512];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct User u;

        int n = sscanf(line, "%d,%100[^,],%20[^,],%100[^,],%20[^,],%d,%d",
                       &u.userID,
                       u.fullName,
                       u.ssn,
                       u.email,
                       u.phone,
                       &u.roleID,
                       &u.companyID);

        if (n == 7) {
            addUserToTable(table, u);
        } else {
            printf("Skipping invalid line: %s", line);
        }
    }

    fclose(fp);


    userHashBuildFromList(table);
}

void saveUserTableToCSV(struct UserTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "userID,fullName,ssn,email,phone,roleID,companyID\n");

    struct UserNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s,%s,%s,%d,%d\n",
                curr->data.userID,
                curr->data.fullName,
                curr->data.ssn,
                curr->data.email,
                curr->data.phone,
                curr->data.roleID,
                curr->data.companyID);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewUser(struct UserTable *users,
               struct RoleTable *roles,
               struct CompanyTable *companies,
               struct User u,
               const char *filename) {

    if (!validateUser(users, roles, companies, u)) {
        return 0;
    }

    struct UserNode *node = malloc(sizeof(struct UserNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = u;
    node->next = NULL;
    node->prev = users->tail;

    if (users->tail != NULL)
        users->tail->next = node;
    else
        users->head = node;

    users->tail = node;
    users->count++;


    userHashInsertNode(node);


    char roleBuf[32], compBuf[32];
    snprintf(roleBuf, sizeof(roleBuf), "%d", u.roleID);
    snprintf(compBuf, sizeof(compBuf), "%d", u.companyID);

    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "fullName",  "", u.fullName,  "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "ssn",       "", u.ssn,       "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "email",     "", u.email,     "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "phone",     "", u.phone,     "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "roleID",    "", roleBuf,     "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, u.userID, "User", "companyID", "", compBuf,     "INSERT");

    saveUserTableToCSV(users, filename);
    return 1;
}

int deleteUserByID(struct UserTable *table,
                   struct VehicleTable *vehicles,
                   int userID) {


    if (isUserUsedByVehicle(vehicles, userID)) {
        printf("Delete blocked: userID %d is referenced by vehicles.\n", userID);
        return 0;
    }

    struct UserNode *curr = userHashFindNode(userID);
    if(curr == NULL){
    printf("User not found.\n");
    return 0;
    }


        if (curr->data.userID == userID) {

            if (curr->prev == NULL)
                table->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            userHashRemoveNode(userID);

            writeAuditLog(AUDIT_LOG_FILE, userID, "User", "*", "", "-", "DELETE");


            free(curr);
            table->count--;

            return 1;
        }

}


int updateUser(struct UserTable *users,
               struct RoleTable *roles,
               struct CompanyTable *companies,
               int userID,
               struct User updated,
               const char *filename) {

    struct UserNode *target = userHashFindNode(userID);

    if (target == NULL) {
        printf("User not found.\n");
        return 0;
    }


    if (strlen(updated.fullName) == 0) {
        printf("Constraint error: fullName cannot be empty\n");
        return 0;
    }


    if (strlen(updated.ssn) == 0) {
        printf("Constraint error: ssn cannot be empty\n");
        return 0;
    }


    if (strlen(updated.email) == 0) {
        printf("Constraint error: email cannot be empty\n");
        return 0;
    }


    if (strlen(updated.phone) == 0) {
        printf("Constraint error: phone cannot be empty\n");
        return 0;
    }


    if (!roleIDExists(roles, updated.roleID)) {
        printf("Constraint error: roleID does not exist\n");
        return 0;
    }
    if (!companyIDExists(companies, updated.companyID)) {
        printf("Constraint error: companyID does not exist\n");
        return 0;
    }


    struct UserNode *curr = users->head;
    while (curr != NULL) {
        if (curr->data.userID != userID &&
            strcmp(curr->data.ssn, updated.ssn) == 0) {
            printf("Constraint error: ssn already exists\n");
            return 0;
        }
        curr = curr->next;
    }


    curr = users->head;
    while (curr != NULL) {
        if (curr->data.userID != userID &&
            strcmp(curr->data.email, updated.email) == 0) {
            printf("Constraint error: email already exists\n");
            return 0;
        }
        curr = curr->next;
    }

    if (strcmp(target->data.fullName, updated.fullName) != 0)
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "fullName",
                      target->data.fullName, updated.fullName, "UPDATE");

    if (strcmp(target->data.ssn, updated.ssn) != 0)
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "ssn",
                      target->data.ssn, updated.ssn, "UPDATE");

    if (strcmp(target->data.email, updated.email) != 0)
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "email",
                      target->data.email, updated.email, "UPDATE");

    if (strcmp(target->data.phone, updated.phone) != 0)
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "phone",
                      target->data.phone, updated.phone, "UPDATE");

    if (target->data.roleID != updated.roleID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.roleID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.roleID);
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "roleID", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.companyID != updated.companyID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.companyID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.companyID);
        writeAuditLog(AUDIT_LOG_FILE, userID, "User", "companyID", oldBuf, newBuf, "UPDATE");
    }



    strcpy(target->data.fullName, updated.fullName);
    strcpy(target->data.ssn, updated.ssn);
    strcpy(target->data.email, updated.email);
    strcpy(target->data.phone, updated.phone);
    target->data.roleID = updated.roleID;
    target->data.companyID = updated.companyID;

    saveUserTableToCSV(users, filename);
    return 1;
}




int getNextVehicleID(struct VehicleTable *table) {
    int maxID = 0;
    struct VehicleNode *curr = table->head;

    while (curr != NULL) {
        if (curr->data.vehicleID > maxID)
            maxID = curr->data.vehicleID;
        curr = curr->next;
    }

    return maxID + 1;
}


// Vehicle validation helpers and CSV I/O.
int vehicleIDExists(struct VehicleTable *t, int vehicleID) {
    (void)t;
    return vehicleHashFindNode(vehicleID) != NULL;
}

int licensePlateExists(struct VehicleTable *t, const char *plate) {
    if (plate == NULL || strlen(plate) == 0) return 0;

    struct VehicleNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.licensePlate, plate) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int validateVehicle(struct VehicleTable *vehicles,
                    struct UserTable *users,
                    struct Vehicle v) {

    if (v.vehicleID <= 0) {
        printf("Constraint error: vehicleID must be > 0\n");
        return 0;
    }

    if (vehicleIDExists(vehicles, v.vehicleID)) {
        printf("Constraint error: vehicleID already exists\n");
        return 0;
    }

    if (strlen(v.licensePlate) == 0 ||
        strlen(v.make) == 0 ||
        strlen(v.model) == 0 ||
        strlen(v.color) == 0 ||
        strlen(v.vehicleType) == 0) {
        printf("Constraint error: all vehicle fields must be NOT NULL\n");
        return 0;
    }

    if (licensePlateExists(vehicles, v.licensePlate)) {
        printf("Constraint error: licensePlate already exists\n");
        return 0;
    }

    if (v.userID <= 0 || !userIDExists(users, v.userID)) {
        printf("Constraint error: userID does not exist\n");
        return 0;
    }

    return 1;
}

int isVehicleUsedByAccessLog(struct AccessLogTable *logs, int vehicleID) {
    struct AccessLogNode *curr = logs->head;
    while (curr != NULL) {
        if (curr->data.vehicleID == vehicleID) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}



void addVehicleToTable(struct VehicleTable *t, struct Vehicle v) {
    struct VehicleNode *node = malloc(sizeof(struct VehicleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = v;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadVehicleTable(struct VehicleTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        vehicleHashInit();
        return;
    }

    char line[512];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct Vehicle v;
        memset(&v, 0, sizeof(v));

        int n = sscanf(line,
            "%d,%20[^,],%50[^,],%50[^,],%30[^,],%30[^,],%d",
            &v.vehicleID,
            v.licensePlate,
            v.make,
            v.model,
            v.color,
            v.vehicleType,
            &v.userID
        );

        if (n == 7) {
            addVehicleToTable(table, v);
        } else {
            printf("Skipping invalid vehicle row: %s", line);
        }
    }

    fclose(fp);


    vehicleHashBuildFromList(table);
}

void saveVehicleTableToCSV(struct VehicleTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "vehicleID,licensePlate,make,model,color,vehicleType,userID\n");

    struct VehicleNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s,%s,%s,%s,%d\n",
                curr->data.vehicleID,
                curr->data.licensePlate,
                curr->data.make,
                curr->data.model,
                curr->data.color,
                curr->data.vehicleType,
                curr->data.userID);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewVehicle(struct VehicleTable *vehicles,
                  struct UserTable *users,
                  struct Vehicle v,
                  const char *filename) {

    if (!validateVehicle(vehicles, users, v)) {
        return 0;
    }

    struct VehicleNode *node = malloc(sizeof(struct VehicleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = v;
    node->next = NULL;
    node->prev = vehicles->tail;

    if (vehicles->tail != NULL)
        vehicles->tail->next = node;
    else
        vehicles->head = node;

    vehicles->tail = node;
    vehicles->count++;


    vehicleHashInsertNode(node);

    char userBuf[32];
    snprintf(userBuf, sizeof(userBuf), "%d", v.userID);

    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "licensePlate", "", v.licensePlate, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "make",         "", v.make,         "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "model",        "", v.model,        "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "color",        "", v.color,        "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "vehicleType",  "", v.vehicleType,  "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, v.vehicleID, "Vehicle", "userID",       "", userBuf,        "INSERT");

    saveVehicleTableToCSV(vehicles, filename);
    return 1;
}

int deleteVehicleByID(struct VehicleTable *table,
                      struct AccessLogTable *logs,
                      int vehicleID) {


    if (isVehicleUsedByAccessLog(logs, vehicleID)) {
        printf("Delete blocked: vehicleID %d is referenced by AccessLog.\n", vehicleID);
        return 0;
    }

    struct VehicleNode *curr = vehicleHashFindNode(vehicleID);
    if(curr == NULL){
    printf("Vehicle not found.\n");
    return 0;
    }

        if (curr->data.vehicleID == vehicleID) {

            if (curr->prev == NULL)
                table->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            vehicleHashRemoveNode(vehicleID);

            writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "*", "", "-", "DELETE");

            free(curr);
            table->count--;

            return 1;
        }

}


int updateVehicle(struct VehicleTable *vehicles,
                  struct UserTable *users,
                  int vehicleID,
                  struct Vehicle updated,
                  const char *filename) {

    struct VehicleNode *target = vehicleHashFindNode(vehicleID);

    if (target == NULL) {
        printf("Vehicle not found.\n");
        return 0;
    }


    if (strlen(updated.licensePlate) == 0 ||
        strlen(updated.make) == 0 ||
        strlen(updated.model) == 0 ||
        strlen(updated.color) == 0 ||
        strlen(updated.vehicleType) == 0) {
        printf("Constraint error: all vehicle fields must be NOT NULL\n");
        return 0;
    }


    if (updated.userID <= 0 || !userIDExists(users, updated.userID)) {
        printf("Constraint error: userID does not exist\n");
        return 0;
    }


    struct VehicleNode *curr = vehicles->head;
    while (curr != NULL) {
        if (curr->data.vehicleID != vehicleID &&
            strcmp(curr->data.licensePlate, updated.licensePlate) == 0) {
            printf("Constraint error: licensePlate already exists\n");
            return 0;
        }
        curr = curr->next;
    }

    if (strcmp(target->data.licensePlate, updated.licensePlate) != 0)
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "licensePlate",
                      target->data.licensePlate, updated.licensePlate, "UPDATE");

    if (strcmp(target->data.make, updated.make) != 0)
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "make",
                      target->data.make, updated.make, "UPDATE");

    if (strcmp(target->data.model, updated.model) != 0)
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "model",
                      target->data.model, updated.model, "UPDATE");

    if (strcmp(target->data.color, updated.color) != 0)
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "color",
                      target->data.color, updated.color, "UPDATE");

    if (strcmp(target->data.vehicleType, updated.vehicleType) != 0)
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "vehicleType",
                      target->data.vehicleType, updated.vehicleType, "UPDATE");

    if (target->data.userID != updated.userID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.userID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.userID);
        writeAuditLog(AUDIT_LOG_FILE, vehicleID, "Vehicle", "userID", oldBuf, newBuf, "UPDATE");
    }


    strcpy(target->data.licensePlate, updated.licensePlate);
    strcpy(target->data.make, updated.make);
    strcpy(target->data.model, updated.model);
    strcpy(target->data.color, updated.color);
    strcpy(target->data.vehicleType, updated.vehicleType);
    target->data.userID = updated.userID;

    saveVehicleTableToCSV(vehicles, filename);
    return 1;
}





// Gate validation helpers and CSV I/O.
int gateIDExists(struct GateTable *t, int gateID) {
    (void)t;
    return gateHashFindNode(gateID) != NULL;
}

int gateNameExists(struct GateTable *t, const char *gateName) {
    if (gateName == NULL || strlen(gateName) == 0) return 0;

    struct GateNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.gateName, gateName) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int validateGate(struct GateTable *gates, struct Gate g) {


    if (g.gateID <= 0) {
        printf("Constraint error: gateID must be > 0\n");
        return 0;
    }
    if (gateIDExists(gates, g.gateID)) {
        printf("Constraint error: gateID already exists\n");
        return 0;
    }


    if (strlen(g.gateName) == 0) {
        printf("Constraint error: gateName cannot be empty\n");
        return 0;
    }
    if (strlen(g.gateType) == 0) {
        printf("Constraint error: gateType cannot be empty\n");
        return 0;
    }


    if (gateNameExists(gates, g.gateName)) {
        printf("Constraint error: gateName already exists\n");
        return 0;
    }

    return 1;
}

int getNextGateID(struct GateTable *table) {
    int maxID = 0;
    struct GateNode *curr = table->head;

    while (curr != NULL) {
        if (curr->data.gateID > maxID)
            maxID = curr->data.gateID;
        curr = curr->next;
    }

    return maxID + 1;
}

int isGateUsedByAccessLog(struct AccessLogTable *logs, int gateID) {
    struct AccessLogNode *curr = logs->head;
    while (curr != NULL) {
        if (curr->data.gateID == gateID) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

int isGateUsedByAccessRule(struct AccessRuleTable *rules, int gateID) {
    struct AccessRuleNode *curr = rules->head;
    while (curr != NULL) {
        if (curr->data.gateID == gateID) return 1;
        curr = curr->next;
    }
    return 0;
}



void addGateToTable(struct GateTable *t, struct Gate g) {
    struct GateNode *node = malloc(sizeof(struct GateNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = g;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadGateTable(struct GateTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        gateHashInit();
        return;
    }

    char line[256];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct Gate g;
        memset(&g, 0, sizeof(g));

        int n = sscanf(line, "%d,%50[^,],%30[^\n]",
                       &g.gateID,
                       g.gateName,
                       g.gateType);

        if (n == 3) {
            addGateToTable(table, g);
        } else {
            printf("Skipping invalid gate row: %s", line);
        }
    }

    fclose(fp);


    gateHashBuildFromList(table);
}

void saveGateTableToCSV(struct GateTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "gateID,gateName,gateType\n");

    struct GateNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s\n",
                curr->data.gateID,
                curr->data.gateName,
                curr->data.gateType);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewGate(struct GateTable *gates, struct Gate g, const char *filename) {

    if (!validateGate(gates, g)) {
        return 0;
    }

    struct GateNode *node = malloc(sizeof(struct GateNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = g;
    node->next = NULL;
    node->prev = gates->tail;

    if (gates->tail != NULL)
        gates->tail->next = node;
    else
        gates->head = node;

    gates->tail = node;
    gates->count++;


    gateHashInsertNode(node);

    writeAuditLog(AUDIT_LOG_FILE, g.gateID, "Gate", "gateName", "", g.gateName, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, g.gateID, "Gate", "gateType", "", g.gateType, "INSERT");

    saveGateTableToCSV(gates, filename);
    return 1;
}

int deleteGateByID(struct GateTable *table,
                   struct AccessLogTable *logs,
                   struct AccessRuleTable *rules,
                   int gateID) {


    if (isGateUsedByAccessLog(logs, gateID)) {
        printf("Delete blocked: gateID %d is referenced by AccessLogs.\n", gateID);
        return 0;
    }


    if (isGateUsedByAccessRule(rules, gateID)) {
        printf("Delete blocked: gateID %d is referenced by AccessRules.\n", gateID);
        return 0;
    }

    struct GateNode *curr = gateHashFindNode(gateID);
    if(curr == NULL){
    printf("Gate not found.\n");
    return 0;
    }

        if (curr->data.gateID == gateID) {

            if (curr->prev == NULL)
                table->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            gateHashRemoveNode(gateID);

            writeAuditLog(AUDIT_LOG_FILE, gateID, "Gate", "*", "", "-", "DELETE");

            free(curr);
            table->count--;

            return 1;
        }

}


int updateGate(struct GateTable *gates,
               int gateID,
               struct Gate updated,
               const char *filename) {

    struct GateNode *target = gateHashFindNode(gateID);

    if (target == NULL) {
        printf("Gate not found.\n");
        return 0;
    }


    if (strlen(updated.gateName) == 0) {
        printf("Constraint error: gateName cannot be empty\n");
        return 0;
    }
    if (strlen(updated.gateType) == 0) {
        printf("Constraint error: gateType cannot be empty\n");
        return 0;
    }


    struct GateNode *curr = gates->head;
    while (curr != NULL) {
        if (curr->data.gateID != gateID &&
            strcmp(curr->data.gateName, updated.gateName) == 0) {
            printf("Constraint error: gateName already exists\n");
            return 0;
        }
        curr = curr->next;
    }

    if (strcmp(target->data.gateName, updated.gateName) != 0)
        writeAuditLog(AUDIT_LOG_FILE, gateID, "Gate", "gateName",
                      target->data.gateName, updated.gateName, "UPDATE");

    if (strcmp(target->data.gateType, updated.gateType) != 0)
        writeAuditLog(AUDIT_LOG_FILE, gateID, "Gate", "gateType",
                      target->data.gateType, updated.gateType, "UPDATE");


    strcpy(target->data.gateName, updated.gateName);
    strcpy(target->data.gateType, updated.gateType);

    saveGateTableToCSV(gates, filename);
    return 1;
}





// Access log validation helpers and CSV I/O.
int accessLogIDExists(struct AccessLogTable *t, int accessLogID) {
    (void)t;
    return accessLogHashFindNode(accessLogID) != NULL;
}

int isValidAccessResult(const char *res) {
    return (strcmp(res, "ALLOWED") == 0 || strcmp(res, "DENIED") == 0);
}

int validateAccessLog(struct AccessLogTable *logs,
                      struct VehicleTable *vehicles,
                      struct GateTable *gates,
                      struct AccessLog a) {


    if (a.accessLogID <= 0) {
        printf("Constraint error: accessLogID must be > 0\n");
        return 0;
    }
    if (accessLogIDExists(logs, a.accessLogID)) {
        printf("Constraint error: accessLogID already exists\n");
        return 0;
    }


    if (strlen(a.accessTime) == 0) {
        printf("Constraint error: accessTime cannot be empty\n");
        return 0;
    }
    if (strlen(a.result) == 0) {
        printf("Constraint error: result cannot be empty\n");
        return 0;
    }
    if (!isValidAccessResult(a.result)) {
        printf("Constraint error: result must be ALLOWED or DENIED\n");
        return 0;
    }


    if (a.vehicleID <= 0 || !vehicleIDExists(vehicles, a.vehicleID)) {
        printf("Constraint error: vehicleID does not exist\n");
        return 0;
    }
    if (a.gateID <= 0 || !gateIDExists(gates, a.gateID)) {
        printf("Constraint error: gateID does not exist\n");
        return 0;
    }

    return 1;
}

int getNextAccessLogID(struct AccessLogTable *table) {
    int maxID = 0;
    struct AccessLogNode *curr = table->head;

    while (curr != NULL) {
        if (curr->data.accessLogID > maxID)
            maxID = curr->data.accessLogID;
        curr = curr->next;
    }
    return maxID + 1;
}



void addAccessLogToTable(struct AccessLogTable *t, struct AccessLog a) {
    struct AccessLogNode *node = malloc(sizeof(struct AccessLogNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = a;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadAccessLogTable(struct AccessLogTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        accessLogHashInit();
        return;
    }

    char line[256];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct AccessLog a;
        memset(&a, 0, sizeof(a));

        int n = sscanf(line, "%d,%19[^,],%10[^,],%d,%d",
                       &a.accessLogID,
                       a.accessTime,
                       a.result,
                       &a.vehicleID,
                       &a.gateID);

        if (n == 5) {
            addAccessLogToTable(table, a);
        } else {
            printf("Skipping invalid access log row: %s", line);
        }
    }

    fclose(fp);


    accessLogHashBuildFromList(table);
}

void saveAccessLogTableToCSV(struct AccessLogTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "accessLogID,accessTime,result,vehicleID,gateID\n");

    struct AccessLogNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s,%d,%d\n",
                curr->data.accessLogID,
                curr->data.accessTime,
                curr->data.result,
                curr->data.vehicleID,
                curr->data.gateID);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewAccessLog(struct AccessLogTable *logs,
                    struct VehicleTable *vehicles,
                    struct GateTable *gates,
                    struct AccessLog a,
                    const char *filename) {

    if (!validateAccessLog(logs, vehicles, gates, a)) {
        return 0;
    }

    struct AccessLogNode *node = malloc(sizeof(struct AccessLogNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = a;
    node->next = NULL;
    node->prev = logs->tail;

    if (logs->tail != NULL)
        logs->tail->next = node;
    else
        logs->head = node;

    logs->tail = node;
    logs->count++;


    accessLogHashInsertNode(node);

    char vehBuf[32], gateBuf[32];
    sprintf(vehBuf, "%d", a.vehicleID);
    sprintf(gateBuf, "%d", a.gateID);

    writeAuditLog(AUDIT_LOG_FILE, a.accessLogID, "AccessLog", "accessTime", "", a.accessTime, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, a.accessLogID, "AccessLog", "result",     "", a.result,     "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, a.accessLogID, "AccessLog", "vehicleID",  "", vehBuf,       "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, a.accessLogID, "AccessLog", "gateID",     "", gateBuf,      "INSERT");

    saveAccessLogTableToCSV(logs, filename);
    return 1;
}

int deleteAccessLogByID(struct AccessLogTable *table, int accessLogID) {
    struct AccessLogNode *curr = accessLogHashFindNode(accessLogID);
    if(curr == NULL){
    printf("Access log not found.\n");
    return 0;
    }

        if (curr->data.accessLogID == accessLogID) {

            if (curr->prev == NULL)
                table->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                table->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            accessLogHashRemoveNode(accessLogID);

            writeAuditLog(AUDIT_LOG_FILE, accessLogID, "AccessLog", "*", "", "-", "DELETE");

            free(curr);
            table->count--;
            return 1;
        }
}


int updateAccessLog(struct AccessLogTable *logs,
                    struct VehicleTable *vehicles,
                    struct GateTable *gates,
                    int accessLogID,
                    struct AccessLog updated,
                    const char *filename) {

    struct AccessLogNode *target = accessLogHashFindNode(accessLogID);

    if (target == NULL) {
        printf("AccessLog not found.\n");
        return 0;
    }

    if (strlen(updated.accessTime) == 0) {
        printf("Constraint error: accessTime cannot be empty\n");
        return 0;
    }

    if (strlen(updated.result) == 0 || !isValidAccessResult(updated.result)) {
        printf("Constraint error: result must be ALLOWED or DENIED\n");
        return 0;
    }

    if (updated.vehicleID <= 0 || !vehicleIDExists(vehicles, updated.vehicleID)) {
        printf("Constraint error: vehicleID does not exist\n");
        return 0;
    }

    if (updated.gateID <= 0 || !gateIDExists(gates, updated.gateID)) {
        printf("Constraint error: gateID does not exist\n");
        return 0;
    }


    if (strcmp(target->data.accessTime, updated.accessTime) != 0)
        writeAuditLog(AUDIT_LOG_FILE, accessLogID, "AccessLog", "accessTime",
                      target->data.accessTime, updated.accessTime, "UPDATE");

    if (strcmp(target->data.result, updated.result) != 0)
        writeAuditLog(AUDIT_LOG_FILE, accessLogID, "AccessLog", "result",
                      target->data.result, updated.result, "UPDATE");

    if (target->data.vehicleID != updated.vehicleID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.vehicleID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.vehicleID);
        writeAuditLog(AUDIT_LOG_FILE, accessLogID, "AccessLog", "vehicleID", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.gateID != updated.gateID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.gateID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.gateID);
        writeAuditLog(AUDIT_LOG_FILE, accessLogID, "AccessLog", "gateID", oldBuf, newBuf, "UPDATE");
    }


    strcpy(target->data.accessTime, updated.accessTime);
    strcpy(target->data.result, updated.result);
    target->data.vehicleID = updated.vehicleID;
    target->data.gateID = updated.gateID;

    saveAccessLogTableToCSV(logs, filename);
    return 1;
}






// Access schedule validation helpers and CSV I/O.
int scheduleIDExists(struct AccessScheduleTable *t, int scheduleID) {
    (void)t;
    return scheduleHashFindNode(scheduleID) != NULL;
}

int scheduleNameExists(struct AccessScheduleTable *t, const char *name) {
    if (name == NULL || strlen(name) == 0) return 0;

    struct AccessScheduleNode *curr = t->head;
    while (curr != NULL) {
        if (strcmp(curr->data.scheduleName, name) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

int validateAccessSchedule(struct AccessScheduleTable *t, struct AccessSchedule s) {


    if (s.scheduleID <= 0) {
        printf("Constraint error: scheduleID must be > 0\n");
        return 0;
    }
    if (scheduleIDExists(t, s.scheduleID)) {
        printf("Constraint error: scheduleID already exists\n");
        return 0;
    }


    if (strlen(s.scheduleName) == 0) {
        printf("Constraint error: scheduleName cannot be empty\n");
        return 0;
    }
    if (strlen(s.daysOfWeek) == 0) {
        printf("Constraint error: daysOfWeek cannot be empty\n");
        return 0;
    }


    if (scheduleNameExists(t, s.scheduleName)) {
        printf("Constraint error: scheduleName already exists\n");
        return 0;
    }

    return 1;
}

int getNextScheduleID(struct AccessScheduleTable *t) {
    int maxID = 0;
    struct AccessScheduleNode *curr = t->head;

    while (curr != NULL) {
        if (curr->data.scheduleID > maxID)
            maxID = curr->data.scheduleID;
        curr = curr->next;
    }
    return maxID + 1;
}

int isScheduleUsedByAccessRule(struct AccessRuleTable *rules, int scheduleID) {
    struct AccessRuleNode *curr = rules->head;
    while (curr != NULL) {
        if (curr->data.scheduleID == scheduleID) return 1;
        curr = curr->next;
    }
    return 0;
}



void addAccessScheduleToTable(struct AccessScheduleTable *t, struct AccessSchedule s) {
    struct AccessScheduleNode *node = malloc(sizeof(struct AccessScheduleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = s;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadAccessScheduleTable(struct AccessScheduleTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        scheduleHashInit();
        return;
    }

    char line[256];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct AccessSchedule s;
        memset(&s, 0, sizeof(s));

        int n = sscanf(line, "%d,%50[^,],%20[^\n]",
                       &s.scheduleID,
                       s.scheduleName,
                       s.daysOfWeek);

        if (n == 3) {
            addAccessScheduleToTable(table, s);
        } else {
            printf("Skipping invalid schedule row: %s", line);
        }
    }

    fclose(fp);


    scheduleHashBuildFromList(table);
}

void saveAccessScheduleTableToCSV(struct AccessScheduleTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "scheduleID,scheduleName,daysOfWeek\n");

    struct AccessScheduleNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%s,%s\n",
                curr->data.scheduleID,
                curr->data.scheduleName,
                curr->data.daysOfWeek);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewAccessSchedule(struct AccessScheduleTable *t,
                         struct AccessSchedule s,
                         const char *filename) {

    if (!validateAccessSchedule(t, s)) {
        return 0;
    }

    struct AccessScheduleNode *node = malloc(sizeof(struct AccessScheduleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = s;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;


    scheduleHashInsertNode(node);

    writeAuditLog(AUDIT_LOG_FILE, s.scheduleID, "AccessSchedule", "scheduleName", "", s.scheduleName, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, s.scheduleID, "AccessSchedule", "daysOfWeek",   "", s.daysOfWeek,   "INSERT");

    saveAccessScheduleTableToCSV(t, filename);
    return 1;
}

int deleteAccessScheduleByID(struct AccessScheduleTable *t,
                             struct AccessRuleTable *rules,
                             int scheduleID) {


    if (isScheduleUsedByAccessRule(rules, scheduleID)) {
        printf("Delete blocked: scheduleID %d is referenced by AccessRules.\n", scheduleID);
        return 0;
    }

    struct AccessScheduleNode *curr = scheduleHashFindNode(scheduleID);
    if(curr == NULL){
    printf("Access schedule not found.\n");
    return 0;
    }

        if (curr->data.scheduleID == scheduleID) {

            if (curr->prev == NULL)
                t->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                t->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            scheduleHashRemoveNode(scheduleID);

            writeAuditLog(AUDIT_LOG_FILE, scheduleID, "AccessSchedule", "*", "", "-", "DELETE");

            free(curr);
            t->count--;
            return 1;
        }
}


int updateAccessSchedule(struct AccessScheduleTable *t,
                         int scheduleID,
                         struct AccessSchedule updated,
                         const char *filename) {

    struct AccessScheduleNode *target = scheduleHashFindNode(scheduleID);

    if (target == NULL) {
        printf("Schedule not found.\n");
        return 0;
    }


    if (strlen(updated.scheduleName) == 0) {
        printf("Constraint error: scheduleName cannot be empty\n");
        return 0;
    }
    if (strlen(updated.daysOfWeek) == 0) {
        printf("Constraint error: daysOfWeek cannot be empty\n");
        return 0;
    }


    struct AccessScheduleNode *curr = t->head;
    while (curr != NULL) {
        if (curr->data.scheduleID != scheduleID &&
            strcmp(curr->data.scheduleName, updated.scheduleName) == 0) {
            printf("Constraint error: scheduleName already exists\n");
            return 0;
        }
        curr = curr->next;
    }


    if (strcmp(target->data.scheduleName, updated.scheduleName) != 0)
        writeAuditLog(AUDIT_LOG_FILE, scheduleID, "AccessSchedule", "scheduleName",
                      target->data.scheduleName, updated.scheduleName, "UPDATE");

    if (strcmp(target->data.daysOfWeek, updated.daysOfWeek) != 0)
        writeAuditLog(AUDIT_LOG_FILE, scheduleID, "AccessSchedule", "daysOfWeek",
                      target->data.daysOfWeek, updated.daysOfWeek, "UPDATE");

    strcpy(target->data.scheduleName, updated.scheduleName);
    strcpy(target->data.daysOfWeek, updated.daysOfWeek);

    saveAccessScheduleTableToCSV(t, filename);
    return 1;
}





// Access rule validation helpers and CSV I/O.
int accessRuleIDExists(struct AccessRuleTable *t, int accessRuleID) {
    (void)t;
    return accessRuleHashFindNode(accessRuleID) != NULL;
}

int validateAccessRule(struct AccessRuleTable *rules,
                       struct RoleTable *roles,
                       struct GateTable *gates,
                       struct AccessScheduleTable *schedules,
                       struct AccessRule r) {


    if (r.accessRuleID <= 0) {
        printf("Constraint error: accessRuleID must be > 0\n");
        return 0;
    }
    if (accessRuleIDExists(rules, r.accessRuleID)) {
        printf("Constraint error: accessRuleID already exists\n");
        return 0;
    }


    if (!(r.isActive == 0 || r.isActive == 1)) {
        printf("Constraint error: isActive must be 0 or 1\n");
        return 0;
    }


    if (r.priority < 0) {
        printf("Constraint error: priority must be >= 0\n");
        return 0;
    }


    if (r.roleID <= 0 || !roleIDExists(roles, r.roleID)) {
        printf("Constraint error: roleID does not exist\n");
        return 0;
    }
    if (r.gateID <= 0 || !gateIDExists(gates, r.gateID)) {
        printf("Constraint error: gateID does not exist\n");
        return 0;
    }
    if (r.scheduleID <= 0 || !scheduleIDExists(schedules, r.scheduleID)) {
        printf("Constraint error: scheduleID does not exist\n");
        return 0;
    }

    return 1;
}

int getNextAccessRuleID(struct AccessRuleTable *t) {
    int maxID = 0;
    struct AccessRuleNode *curr = t->head;

    while (curr != NULL) {
        if (curr->data.accessRuleID > maxID)
            maxID = curr->data.accessRuleID;
        curr = curr->next;
    }
    return maxID + 1;
}



void addAccessRuleToTable(struct AccessRuleTable *t, struct AccessRule r) {
    struct AccessRuleNode *node = malloc(sizeof(struct AccessRuleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    node->data = r;
    node->next = NULL;
    node->prev = t->tail;

    if (t->tail != NULL)
        t->tail->next = node;
    else
        t->head = node;

    t->tail = node;
    t->count++;
}

void loadAccessRuleTable(struct AccessRuleTable *table, const char *filename) {

    table->head = NULL;
    table->tail = NULL;
    table->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        accessRuleHashInit();
        return;
    }

    char line[256];


    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        struct AccessRule r;
        memset(&r, 0, sizeof(r));

        int n = sscanf(line, "%d,%d,%d,%d,%d,%d",
                       &r.accessRuleID,
                       &r.isActive,
                       &r.priority,
                       &r.roleID,
                       &r.gateID,
                       &r.scheduleID);

        if (n == 6) {
            addAccessRuleToTable(table, r);
        } else {
            printf("Skipping invalid access rule row: %s", line);
        }
    }

    fclose(fp);


    accessRuleHashBuildFromList(table);
}

void saveAccessRuleTableToCSV(struct AccessRuleTable *t, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "accessRuleID,isActive,priority,roleID,gateID,scheduleID\n");

    struct AccessRuleNode *curr = t->head;
    while (curr != NULL) {
        fprintf(fp, "%d,%d,%d,%d,%d,%d\n",
                curr->data.accessRuleID,
                curr->data.isActive,
                curr->data.priority,
                curr->data.roleID,
                curr->data.gateID,
                curr->data.scheduleID);
        curr = curr->next;
    }

    fclose(fp);
}


int addNewAccessRule(struct AccessRuleTable *rules,
                     struct RoleTable *roles,
                     struct GateTable *gates,
                     struct AccessScheduleTable *schedules,
                     struct AccessRule r,
                     const char *filename) {

    if (!validateAccessRule(rules, roles, gates, schedules, r)) {
        return 0;
    }

    struct AccessRuleNode *node = malloc(sizeof(struct AccessRuleNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    node->data = r;
    node->next = NULL;
    node->prev = rules->tail;

    if (rules->tail != NULL)
        rules->tail->next = node;
    else
        rules->head = node;

    rules->tail = node;
    rules->count++;


    accessRuleHashInsertNode(node);
    //change from int to char
    char b1[32], b2[32], b3[32], b4[32], b5[32];
    sprintf(b1, "%d", r.isActive);
    sprintf(b2, "%d", r.priority);
    sprintf(b3, "%d", r.roleID);
    sprintf(b4, "%d", r.gateID);
    sprintf(b5, "%d", r.scheduleID);

    writeAuditLog(AUDIT_LOG_FILE, r.accessRuleID, "AccessRule", "isActive",   "", b1, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, r.accessRuleID, "AccessRule", "priority",   "", b2, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, r.accessRuleID, "AccessRule", "roleID",     "", b3, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, r.accessRuleID, "AccessRule", "gateID",     "", b4, "INSERT");
    writeAuditLog(AUDIT_LOG_FILE, r.accessRuleID, "AccessRule", "scheduleID", "", b5, "INSERT");

    saveAccessRuleTableToCSV(rules, filename);
    return 1;
}

int deleteAccessRuleByID(struct AccessRuleTable *t, int accessRuleID) {
    struct AccessRuleNode *curr = accessRuleHashFindNode(accessRuleID);
    if(curr == NULL){
    printf("Access rule not found.\n");
    return 0;
    }


        if (curr->data.accessRuleID == accessRuleID) {

            if (curr->prev == NULL)
                t->head = curr->next;
            else
                curr->prev->next = curr->next;

            if (curr->next == NULL)
                t->tail = curr->prev;
            else
                curr->next->prev = curr->prev;


            accessRuleHashRemoveNode(accessRuleID);

            writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "*", "", "-", "DELETE");

            free(curr);
            t->count--;
            return 1;
        }
}


int updateAccessRule(struct AccessRuleTable *rules,
                     struct RoleTable *roles,
                     struct GateTable *gates,
                     struct AccessScheduleTable *schedules,
                     int accessRuleID,
                     struct AccessRule updated,
                     const char *filename) {

    struct AccessRuleNode *target = accessRuleHashFindNode(accessRuleID);

    if (target == NULL) {
        printf("AccessRule not found.\n");
        return 0;
    }

    if (!(updated.isActive == 0 || updated.isActive == 1)) {
        printf("Constraint error: isActive must be 0 or 1\n");
        return 0;
    }

    if (updated.priority < 0) {
        printf("Constraint error: priority must be >= 0\n");
        return 0;
    }

    if (updated.roleID <= 0 || !roleIDExists(roles, updated.roleID)) {
        printf("Constraint error: roleID does not exist\n");
        return 0;
    }
    if (updated.gateID <= 0 || !gateIDExists(gates, updated.gateID)) {
        printf("Constraint error: gateID does not exist\n");
        return 0;
    }
    if (updated.scheduleID <= 0 || !scheduleIDExists(schedules, updated.scheduleID)) {
        printf("Constraint error: scheduleID does not exist\n");
        return 0;
    }


    if (target->data.isActive != updated.isActive) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.isActive);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.isActive);
        writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "isActive", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.priority != updated.priority) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.priority);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.priority);
        writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "priority", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.roleID != updated.roleID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.roleID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.roleID);
        writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "roleID", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.gateID != updated.gateID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.gateID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.gateID);
        writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "gateID", oldBuf, newBuf, "UPDATE");
    }

    if (target->data.scheduleID != updated.scheduleID) {
        char oldBuf[32], newBuf[32];
        snprintf(oldBuf, sizeof(oldBuf), "%d", target->data.scheduleID);
        snprintf(newBuf, sizeof(newBuf), "%d", updated.scheduleID);
        writeAuditLog(AUDIT_LOG_FILE, accessRuleID, "AccessRule", "scheduleID", oldBuf, newBuf, "UPDATE");
    }


    target->data.isActive = updated.isActive;
    target->data.priority = updated.priority;
    target->data.roleID = updated.roleID;
    target->data.gateID = updated.gateID;
    target->data.scheduleID = updated.scheduleID;

    saveAccessRuleTableToCSV(rules, filename);
    return 1;
}




// back up
void backupAllTablesToCSV(struct CompanyTable *companies,
                          struct RoleTable *roles,
                          struct UserTable *users,
                          struct VehicleTable *vehicles,
                          struct GateTable *gates,
                          struct AccessScheduleTable *schedules,
                          struct AccessRuleTable *rules,
                          struct AccessLogTable *accessLogs) {

    saveCompanyTableToCSV(companies, "companies_backup.csv");
    saveRoleTableToCSV(roles, "roles_backup.csv");
    saveUserTableToCSV(users, "users_backup.csv");
    saveVehicleTableToCSV(vehicles, "vehicles_backup.csv");
    saveGateTableToCSV(gates, "gates_backup.csv");
    saveAccessScheduleTableToCSV(schedules, "access_schedules_backup.csv");
    saveAccessRuleTableToCSV(rules, "access_rules_backup.csv");
    saveAccessLogTableToCSV(accessLogs, "access_logs_backup.csv");

    printf("Backup CSV files created.\n");
}



// print tables
void printCompanyTable(struct CompanyTable *t) {
    struct CompanyNode *curr = t->head;
    printf("=== Companies ===\n");
    while (curr != NULL) {
        printf("ID=%d | Name=%s | Phone=%s | Email=%s\n",
               curr->data.companyID,
               curr->data.companyName,
               curr->data.phone,
               curr->data.email);
        curr = curr->next;
    }
}
void printRoleTable(struct RoleTable *t) {
    struct RoleNode *curr = t->head;

    printf("=== Roles ===\n");
    while (curr != NULL) {
        printf("ID=%d | Name=%s | Description=%s\n",
               curr->data.roleID,
               curr->data.roleName,
               curr->data.description);
        curr = curr->next;
    }
}

void printUserTable(struct UserTable *t) {
    struct UserNode *curr = t->head;

    printf("=== Users ===\n");
    while (curr != NULL) {
        printf("ID=%d | Name=%s | SSN=%s | Email=%s | Phone=%s | roleID=%d | companyID=%d\n",
               curr->data.userID,
               curr->data.fullName,
               curr->data.ssn,
               curr->data.email,
               curr->data.phone,
               curr->data.roleID,
               curr->data.companyID);
        curr = curr->next;
    }
}

void printVehicleTable(struct VehicleTable *t) {
    struct VehicleNode *curr = t->head;

    printf("=== Vehicles ===\n");
    while (curr != NULL) {
        printf("ID=%d | Plate=%s | Make=%s | Model=%s | Color=%s | Type=%s | UserID=%d\n",
               curr->data.vehicleID,
               curr->data.licensePlate,
               curr->data.make,
               curr->data.model,
               curr->data.color,
               curr->data.vehicleType,
               curr->data.userID);
        curr = curr->next;
    }
}

void printGateTable(struct GateTable *t) {
    struct GateNode *curr = t->head;

    printf("=== Gates ===\n");
    while (curr != NULL) {
        printf("ID=%d | Name=%s | Type=%s\n",
               curr->data.gateID,
               curr->data.gateName,
               curr->data.gateType);
        curr = curr->next;
    }
}

void printAccessLogTable(struct AccessLogTable *t) {
    struct AccessLogNode *curr = t->head;

    printf("=== Access Logs ===\n");
    while (curr != NULL) {
        printf("ID=%d | Time=%s | Result=%s | VehicleID=%d | GateID=%d\n",
               curr->data.accessLogID,
               curr->data.accessTime,
               curr->data.result,
               curr->data.vehicleID,
               curr->data.gateID);
        curr = curr->next;
    }
}

void printAccessScheduleTable(struct AccessScheduleTable *t) {
    struct AccessScheduleNode *curr = t->head;

    printf("=== Access Schedules ===\n");
    while (curr != NULL) {
        printf("ID=%d | Name=%s | Days=%s\n",
               curr->data.scheduleID,
               curr->data.scheduleName,
               curr->data.daysOfWeek);
        curr = curr->next;
    }
}


void printAccessRuleTable(struct AccessRuleTable *t) {
    struct AccessRuleNode *curr = t->head;

    printf("=== Access Rules ===\n");
    while (curr != NULL) {
        printf("ID=%d | Active=%d | Priority=%d | RoleID=%d | GateID=%d | ScheduleID=%d\n",
               curr->data.accessRuleID,
               curr->data.isActive,
               curr->data.priority,
               curr->data.roleID,
               curr->data.gateID,
               curr->data.scheduleID);
        curr = curr->next;
    }
}



// main helpers
static void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

static int readInt() {
    char buf[64];
    readLine(buf, sizeof(buf));
    return atoi(buf);
}



// CC
typedef enum { LOCK_NONE, LOCK_S, LOCK_X } LockMode;
typedef enum { TABLE_COMPANY = 1, TABLE_USER = 2 } TableChoice;

typedef enum {
    OP_SELECT = 1,
    OP_INSERT,
    OP_UPDATE,
    OP_DELETE
} OperationType;


static LockMode companyLockMode = LOCK_NONE;
static int companySCount = 0;
static pthread_mutex_t companyLockMtx = PTHREAD_MUTEX_INITIALIZER;


static LockMode userLockMode = LOCK_NONE;
static int userSCount = 0;
static pthread_mutex_t userLockMtx = PTHREAD_MUTEX_INITIALIZER;


struct SimRequest {
    int simUserID;
    TableChoice table;
    OperationType op;

    struct CompanyTable *companies;
    struct RoleTable *roles;
    struct UserTable *users;
    struct VehicleTable *vehicles;
};


void acquireCompanyS(int userID) {
    for (;;) {
        pthread_mutex_lock(&companyLockMtx);

        if (companyLockMode != LOCK_X) {
            if (companyLockMode == LOCK_NONE) companyLockMode = LOCK_S;
            companySCount++;
            printf("User %d started SELECT on Company\n", userID);
            pthread_mutex_unlock(&companyLockMtx);
            return;
        }

        pthread_mutex_unlock(&companyLockMtx);
        printf("User %d waits (S lock)\n", userID);
        sleep(1);
    }
}

void acquireCompanyX(int userID, const char *op) {
    for (;;) {
        pthread_mutex_lock(&companyLockMtx);

        if (companyLockMode == LOCK_NONE) {
            companyLockMode = LOCK_X;
            printf("User %d started %s on Company\n", userID, op);
            pthread_mutex_unlock(&companyLockMtx);
            return;
        }

        pthread_mutex_unlock(&companyLockMtx);
        printf("User %d waits (X lock)\n", userID);
        sleep(1);
    }
}

void releaseCompanyS(int userID) {
    pthread_mutex_lock(&companyLockMtx);
    companySCount--;
    if (companySCount <= 0) {
        companySCount = 0;
        companyLockMode = LOCK_NONE;
    }
    pthread_mutex_unlock(&companyLockMtx);
    printf("User %d finished SELECT, released lock\n", userID);
}

void releaseCompanyX(int userID, const char *op) {
    pthread_mutex_lock(&companyLockMtx);
    companyLockMode = LOCK_NONE;
    pthread_mutex_unlock(&companyLockMtx);
    printf("User %d finished %s, released lock\n", userID, op);
}


void acquireUserS(int userID) {
    for (;;) {
        pthread_mutex_lock(&userLockMtx);

        if (userLockMode != LOCK_X) {
            if (userLockMode == LOCK_NONE) userLockMode = LOCK_S;
            userSCount++;
            printf("User %d started SELECT on User\n", userID);
            pthread_mutex_unlock(&userLockMtx);
            return;
        }

        pthread_mutex_unlock(&userLockMtx);
        printf("User %d waits (S lock)\n", userID);
        sleep(1);
    }
}

void acquireUserX(int userID, const char *op) {
    for (;;) {
        pthread_mutex_lock(&userLockMtx);

        if (userLockMode == LOCK_NONE) {
            userLockMode = LOCK_X;
            printf("User %d started %s on User\n", userID, op);
            pthread_mutex_unlock(&userLockMtx);
            return;
        }

        pthread_mutex_unlock(&userLockMtx);
        printf("User %d waits (X lock)\n", userID);
        sleep(1);
    }
}

void releaseUserS(int userID) {
    pthread_mutex_lock(&userLockMtx);
    userSCount--;
    if (userSCount <= 0) {
        userSCount = 0;
        userLockMode = LOCK_NONE;
    }
    pthread_mutex_unlock(&userLockMtx);
    printf("User %d finished SELECT, released lock\n", userID);
}

void releaseUserX(int userID, const char *op) {
    pthread_mutex_lock(&userLockMtx);
    userLockMode = LOCK_NONE;
    pthread_mutex_unlock(&userLockMtx);
    printf("User %d finished %s, released lock\n", userID, op);
}


// thread function
void* simThread(void *arg) {
    struct SimRequest *req = (struct SimRequest*)arg;


    if (req->table == TABLE_COMPANY) {

        if (req->op == OP_SELECT) {
            acquireCompanyS(req->simUserID);
            printCompanyTable(req->companies);
            sleep(2);
            releaseCompanyS(req->simUserID);
        }
        else if (req->op == OP_INSERT) {
            acquireCompanyX(req->simUserID, "INSERT");

            struct Company c = (struct Company){0};
            c.companyID = 9000 + req->simUserID;

            snprintf(c.companyName, sizeof(c.companyName), "SimCompany_%d", req->simUserID);
            snprintf(c.phone, sizeof(c.phone), "070%04d", req->simUserID);
            snprintf(c.email, sizeof(c.email), "simcompany_%d@demo.com", req->simUserID);

            addNewCompany(req->companies, c, "companies.csv");

            sleep(2);
            releaseCompanyX(req->simUserID, "INSERT");
        }
        else if (req->op == OP_UPDATE) {
            acquireCompanyX(req->simUserID, "UPDATE");

            int id =2;

            struct Company upd = (struct Company){0};
            snprintf(upd.companyName, sizeof(upd.companyName), "UpdCompany_%d", req->simUserID);
            snprintf(upd.phone, sizeof(upd.phone), "079%04d", req->simUserID);
            snprintf(upd.email, sizeof(upd.email), "updcompany_%d@demo.com", req->simUserID);

            updateCompany(req->companies, id, upd, "companies.csv");

            sleep(2);
            releaseCompanyX(req->simUserID, "UPDATE");
        }
        else if (req->op == OP_DELETE) {
            acquireCompanyX(req->simUserID, "DELETE");

            deleteCompanyByID(req->companies, req->users, 1);
            saveCompanyTableToCSV(req->companies, "companies.csv");

            sleep(2);
            releaseCompanyX(req->simUserID, "DELETE");
        }

        return NULL;
    }


    if (req->table == TABLE_USER) {

        if (req->op == OP_SELECT) {
            acquireUserS(req->simUserID);
            printUserTable(req->users);
            sleep(2);
            releaseUserS(req->simUserID);
        }
        else if (req->op == OP_INSERT) {
            acquireUserX(req->simUserID, "INSERT");

            struct User u = (struct User){0};
            u.userID = 8000 + req->simUserID;

            snprintf(u.fullName, sizeof(u.fullName), "SimUser_%d", req->simUserID);
            snprintf(u.ssn, sizeof(u.ssn), "SIMSSN_%d", req->simUserID);
            snprintf(u.email, sizeof(u.email), "simuser_%d@demo.com", req->simUserID);
            snprintf(u.phone, sizeof(u.phone), "078%04d", req->simUserID);


            u.roleID = 1;
            u.companyID = 1;

            addNewUser(req->users, req->roles, req->companies, u, "users.csv");

            sleep(2);
            releaseUserX(req->simUserID, "INSERT");
        }
        else if (req->op == OP_UPDATE) {
            acquireUserX(req->simUserID, "UPDATE");

            int id = 1;

            struct User upd = (struct User){0};
            snprintf(upd.fullName, sizeof(upd.fullName), "UpdUser_%d", req->simUserID);
            snprintf(upd.ssn, sizeof(upd.ssn), "UPDSSN_%d", req->simUserID);
            snprintf(upd.email, sizeof(upd.email), "upduser_%d@demo.com", req->simUserID);
            snprintf(upd.phone, sizeof(upd.phone), "079%04d", req->simUserID);

            upd.roleID = 1;
            upd.companyID = 1;

            updateUser(req->users, req->roles, req->companies, id, upd, "users.csv");

            sleep(2);
            releaseUserX(req->simUserID, "UPDATE");
        }
        else if (req->op == OP_DELETE) {
            acquireUserX(req->simUserID, "DELETE");


            deleteUserByID(req->users, req->vehicles, 1);
            saveUserTableToCSV(req->users, "users.csv");

            sleep(2);
            releaseUserX(req->simUserID, "DELETE");
        }

        return NULL;
    }

    return NULL;
}


// Entry point for the concurrency simulation menu option.
void runConcurrencySimulation(struct CompanyTable *companies,
                              struct RoleTable *roles,
                              struct UserTable *users,
                              struct VehicleTable *vehicles) {

    int n;
    printf("Number of users (max 5): ");
    n = readInt();
    if (n < 1 || n > 5) {
        printf("Invalid number.\n");
        return;
    }

    int tableChoice;
    printf("Choose table:\n1) Company\n2) User\nChoose: ");
    tableChoice = readInt();
    if (tableChoice != 1 && tableChoice != 2) {
        printf("Invalid table.\n");
        return;
    }

    pthread_t threads[5];
    struct SimRequest reqs[5];


    for (int i = 0; i < n; i++) {
        int op;
        printf("\nUser %d operation:\n1) SELECT\n2) INSERT\n3) UPDATE\n4) DELETE\nChoose: ", i + 1);
        op = readInt();
        if (op < 1 || op > 4) op = 1;

        reqs[i].simUserID = i + 1;
        reqs[i].table = (TableChoice)tableChoice;
        reqs[i].op = (OperationType)op;

        reqs[i].companies = companies;
        reqs[i].roles = roles;
        reqs[i].users = users;
        reqs[i].vehicles = vehicles;
    }

    printf("\n--- Starting all users concurrently ---\n");


    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, simThread, &reqs[i]);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Simulation finished.\n");
}

// UI
int main(void) {




    createCompanyCSVHeader("companies.csv");
    createRoleCSVHeader("roles.csv");
    createUserCSVHeader("users.csv");
    createVehicleCSVHeader("vehicles.csv");
    createGateCSVHeader("gates.csv");
    createAccessScheduleCSVHeader("access_schedules.csv");
    createAccessRuleCSVHeader("access_rules.csv");
    createAccessLogCSVHeader("access_logs.csv");


    struct CompanyTable companies;
    struct RoleTable roles;
    struct UserTable users;
    struct VehicleTable vehicles;
    struct GateTable gates;
    struct AccessScheduleTable schedules;
    struct AccessRuleTable rules;
    struct AccessLogTable accessLogs;


    loadCompanyTable(&companies, "companies.csv");
    loadRoleTable(&roles, "roles.csv");
    loadUserTable(&users, "users.csv");
    loadVehicleTable(&vehicles, "vehicles.csv");
    loadGateTable(&gates, "gates.csv");
    loadAccessScheduleTable(&schedules, "access_schedules.csv");
    loadAccessRuleTable(&rules, "access_rules.csv");
    loadAccessLogTable(&accessLogs, "access_logs.csv");

    int tableChoice = -1;

    while (tableChoice != 0) {

        printf("\n=========== TABLES ===========\n");
        printf("1) Companies\n");
        printf("2) Roles\n");
        printf("3) Users\n");
        printf("4) Vehicles\n");
        printf("5) Gates\n");
        printf("6) Access Schedules\n");
        printf("7) Access Rules\n");
        printf("8) Access Logs\n");
        printf("9) Backup All Tables\n");
        printf("10) Concurrency Control\n");
        printf("11) View Audit Log\n");
        printf("0) Exit\n");
        printf("Choose table: ");

        tableChoice = readInt();

        if (tableChoice < 0 || tableChoice > 11) {
            printf("Invalid table choice.\n");
            continue;
        }

        if (tableChoice == 0) break;

        if (tableChoice == 9) {
            backupAllTablesToCSV(&companies, &roles, &users, &vehicles, &gates,
                         &schedules, &rules, &accessLogs);
            continue;
        }

        if (tableChoice == 10) {
            runConcurrencySimulation(&companies, &roles, &users, &vehicles);
            continue;
        }

        if(tableChoice == 11){
            printAuditLog(AUDIT_LOG_FILE);
            continue;
        }


        int actionChoice = -1;

        while (actionChoice != 0) {

            printf("\n----------- ACTIONS -----------\n");
            printf("1) List\n");
            printf("2) Add\n");
            printf("3) Update\n");
            printf("4) Delete\n");
            printf("0) Back\n");
            printf("Choose action: ");

            actionChoice = readInt();

            if (actionChoice == 0) break;

            //company menu
            if (tableChoice == 1) {

                if (actionChoice == 1) {
                    printCompanyTable(&companies);
                }
                else if (actionChoice == 2) {
                    struct Company c = (struct Company){0};
                    c.companyID = getNextCompanyID(&companies);

                    printf("Company name: ");
                    readLine(c.companyName, sizeof(c.companyName));
                    printf("Phone: ");
                    readLine(c.phone, sizeof(c.phone));
                    printf("Email: ");
                    readLine(c.email, sizeof(c.email));

                    addNewCompany(&companies, c, "companies.csv");
                }
                else if (actionChoice == 3) {
                    struct Company upd = (struct Company){0};
                    int id;

                    printf("CompanyID: ");
                    id = readInt();
                    printf("New name: ");
                    readLine(upd.companyName, sizeof(upd.companyName));
                    printf("New phone: ");
                    readLine(upd.phone, sizeof(upd.phone));
                    printf("New email: ");
                    readLine(upd.email, sizeof(upd.email));

                    updateCompany(&companies, id, upd, "companies.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("CompanyID: ");
                    id = readInt();

                    if (deleteCompanyByID(&companies, &users, id)) {
                        saveCompanyTableToCSV(&companies, "companies.csv");
                        printf("Company deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            // role menu
            else if (tableChoice == 2) {

                if (actionChoice == 1) {
                    printRoleTable(&roles);
                }
                else if (actionChoice == 2) {
                    struct Role r = (struct Role){0};
                    r.roleID = getNextRoleID(&roles);

                    printf("Role name: ");
                    readLine(r.roleName, sizeof(r.roleName));
                    printf("Description: ");
                    readLine(r.description, sizeof(r.description));

                    addNewRole(&roles, r, "roles.csv");
                }
                else if (actionChoice == 3) {
                    struct Role upd = (struct Role){0};
                    int id;

                    printf("RoleID: ");
                    id = readInt();
                    printf("New name: ");
                    readLine(upd.roleName, sizeof(upd.roleName));
                    printf("New description: ");
                    readLine(upd.description, sizeof(upd.description));

                    updateRole(&roles, id, upd, "roles.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("RoleID: ");
                    id = readInt();

                    if (deleteRoleByID(&roles, &users, &rules, id)) {
                        saveRoleTableToCSV(&roles, "roles.csv");
                        printf("Role deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            //user menu
            else if (tableChoice == 3) {

                if (actionChoice == 1) {
                    printUserTable(&users);
                }
                else if (actionChoice == 2) {
                    struct User u = (struct User){0};
                    u.userID = getNextUserID(&users);

                    printf("Full name: ");
                    readLine(u.fullName, sizeof(u.fullName));
                    printf("SSN: ");
                    readLine(u.ssn, sizeof(u.ssn));
                    printf("Email: ");
                    readLine(u.email, sizeof(u.email));
                    printf("Phone: ");
                    readLine(u.phone, sizeof(u.phone));
                    printf("RoleID: ");
                    u.roleID = readInt();
                    printf("CompanyID: ");
                    u.companyID = readInt();

                    addNewUser(&users, &roles, &companies, u, "users.csv");
                }
                else if (actionChoice == 3) {
                    struct User upd = (struct User){0};
                    int id;

                    printf("UserID: ");
                    id = readInt();
                    printf("New name: ");
                    readLine(upd.fullName, sizeof(upd.fullName));
                    printf("New SSN: ");
                    readLine(upd.ssn, sizeof(upd.ssn));
                    printf("New email: ");
                    readLine(upd.email, sizeof(upd.email));
                    printf("New phone: ");
                    readLine(upd.phone, sizeof(upd.phone));
                    printf("New roleID: ");
                    upd.roleID = readInt();
                    printf("New companyID: ");
                    upd.companyID = readInt();

                    updateUser(&users, &roles, &companies, id, upd, "users.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("UserID: ");
                    id = readInt();

                    if (deleteUserByID(&users, &vehicles, id)) {
                        saveUserTableToCSV(&users, "users.csv");
                        printf("User deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            // vehicle menu
            else if (tableChoice == 4) {

                if (actionChoice == 1) {
                    printVehicleTable(&vehicles);
                }
                else if (actionChoice == 2) {
                    struct Vehicle v = (struct Vehicle){0};
                    v.vehicleID = getNextVehicleID(&vehicles);

                    printf("License plate: ");
                    readLine(v.licensePlate, sizeof(v.licensePlate));
                    printf("Make: ");
                    readLine(v.make, sizeof(v.make));
                    printf("Model: ");
                    readLine(v.model, sizeof(v.model));
                    printf("Color: ");
                    readLine(v.color, sizeof(v.color));
                    printf("Type: ");
                    readLine(v.vehicleType, sizeof(v.vehicleType));
                    printf("UserID: ");
                    v.userID = readInt();

                    addNewVehicle(&vehicles, &users, v, "vehicles.csv");
                }
                else if (actionChoice == 3) {
                    struct Vehicle upd = (struct Vehicle){0};
                    int id;

                    printf("VehicleID: ");
                    id = readInt();
                    printf("New license plate: ");
                    readLine(upd.licensePlate, sizeof(upd.licensePlate));
                    printf("New make: ");
                    readLine(upd.make, sizeof(upd.make));
                    printf("New model: ");
                    readLine(upd.model, sizeof(upd.model));
                    printf("New color: ");
                    readLine(upd.color, sizeof(upd.color));
                    printf("New type: ");
                    readLine(upd.vehicleType, sizeof(upd.vehicleType));
                    printf("New userID: ");
                    upd.userID = readInt();

                    updateVehicle(&vehicles, &users, id, upd, "vehicles.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("VehicleID: ");
                    id = readInt();

                    if (deleteVehicleByID(&vehicles, &accessLogs, id)) {
                        saveVehicleTableToCSV(&vehicles, "vehicles.csv");
                        printf("Vehicle deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            //gate menu
            else if (tableChoice == 5) {

                if (actionChoice == 1) {
                    printGateTable(&gates);
                }
                else if (actionChoice == 2) {
                    struct Gate g = (struct Gate){0};
                    g.gateID = getNextGateID(&gates);

                    printf("Gate name: ");
                    readLine(g.gateName, sizeof(g.gateName));
                    printf("Gate type: ");
                    readLine(g.gateType, sizeof(g.gateType));

                    addNewGate(&gates, g, "gates.csv");
                }
                else if (actionChoice == 3) {
                    struct Gate upd = (struct Gate){0};
                    int id;

                    printf("GateID: ");
                    id = readInt();
                    printf("New gate name: ");
                    readLine(upd.gateName, sizeof(upd.gateName));
                    printf("New gate type: ");
                    readLine(upd.gateType, sizeof(upd.gateType));

                    updateGate(&gates, id, upd, "gates.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("GateID: ");
                    id = readInt();

                    if (deleteGateByID(&gates, &accessLogs, &rules, id)) {
                        saveGateTableToCSV(&gates, "gates.csv");
                        printf("Gate deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            // AccessSchedule menu
            else if (tableChoice == 6) {

                if (actionChoice == 1) {
                    printAccessScheduleTable(&schedules);
                }
                else if (actionChoice == 2) {
                    struct AccessSchedule s = (struct AccessSchedule){0};
                    s.scheduleID = getNextScheduleID(&schedules);

                    printf("Schedule name: ");
                    readLine(s.scheduleName, sizeof(s.scheduleName));
                    printf("Days: ");
                    readLine(s.daysOfWeek, sizeof(s.daysOfWeek));

                    addNewAccessSchedule(&schedules, s, "access_schedules.csv");
                }
                else if (actionChoice == 3) {
                    struct AccessSchedule upd = (struct AccessSchedule){0};
                    int id;

                    printf("ScheduleID: ");
                    id = readInt();
                    printf("New schedule name: ");
                    readLine(upd.scheduleName, sizeof(upd.scheduleName));
                    printf("New days: ");
                    readLine(upd.daysOfWeek, sizeof(upd.daysOfWeek));

                    updateAccessSchedule(&schedules, id, upd, "access_schedules.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("ScheduleID: ");
                    id = readInt();

                    if (deleteAccessScheduleByID(&schedules, &rules, id)) {
                        saveAccessScheduleTableToCSV(&schedules, "access_schedules.csv");
                        printf("Schedule deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            // AccessRule menu
            else if (tableChoice == 7) {

                if (actionChoice == 1) {
                    printAccessRuleTable(&rules);
                }
                else if (actionChoice == 2) {
                    struct AccessRule r = (struct AccessRule){0};
                    r.accessRuleID = getNextAccessRuleID(&rules);

                    printf("isActive (0/1): ");
                    r.isActive = readInt();
                    printf("Priority: ");
                    r.priority = readInt();
                    printf("RoleID: ");
                    r.roleID = readInt();
                    printf("GateID: ");
                    r.gateID = readInt();
                    printf("ScheduleID: ");
                    r.scheduleID = readInt();

                    addNewAccessRule(&rules, &roles, &gates, &schedules, r, "access_rules.csv");
                }
                else if (actionChoice == 3) {
                    struct AccessRule upd = (struct AccessRule){0};
                    int id;

                    printf("AccessRuleID: ");
                    id = readInt();
                    printf("New isActive (0/1): ");
                    upd.isActive = readInt();
                    printf("New priority: ");
                    upd.priority = readInt();
                    printf("New RoleID: ");
                    upd.roleID = readInt();
                    printf("New GateID: ");
                    upd.gateID = readInt();
                    printf("New ScheduleID: ");
                    upd.scheduleID = readInt();

                    updateAccessRule(&rules, &roles, &gates, &schedules, id, upd, "access_rules.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("AccessRuleID: ");
                    id = readInt();

                    if (deleteAccessRuleByID(&rules, id)) {
                        saveAccessRuleTableToCSV(&rules, "access_rules.csv");
                        printf("AccessRule deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }

            // AccessLog menu
            else if (tableChoice == 8) {

                if (actionChoice == 1) {
                    printAccessLogTable(&accessLogs);
                }
                else if (actionChoice == 2) {
                    struct AccessLog a = (struct AccessLog){0};
                    a.accessLogID = getNextAccessLogID(&accessLogs);

                    printf("Access time: ");
                    readLine(a.accessTime, sizeof(a.accessTime));
                    printf("Result (ALLOWED/DENIED): ");
                    readLine(a.result, sizeof(a.result));
                    printf("VehicleID: ");
                    a.vehicleID = readInt();
                    printf("GateID: ");
                    a.gateID = readInt();

                    addNewAccessLog(&accessLogs, &vehicles, &gates, a, "access_logs.csv");
                }
                else if (actionChoice == 3) {
                    struct AccessLog upd = (struct AccessLog){0};
                    int id;

                    printf("AccessLogID: ");
                    id = readInt();
                    printf("New access time: ");
                    readLine(upd.accessTime, sizeof(upd.accessTime));
                    printf("New result (ALLOWED/DENIED): ");
                    readLine(upd.result, sizeof(upd.result));
                    printf("New VehicleID: ");
                    upd.vehicleID = readInt();
                    printf("New GateID: ");
                    upd.gateID = readInt();

                    updateAccessLog(&accessLogs, &vehicles, &gates, id, upd, "access_logs.csv");
                }
                else if (actionChoice == 4) {
                    int id;

                    printf("AccessLogID: ");
                    id = readInt();

                    if (deleteAccessLogByID(&accessLogs, id)) {
                        saveAccessLogTableToCSV(&accessLogs, "access_logs.csv");
                        printf("AccessLog deleted.\n");
                    }
                }
                else {
                    printf("Invalid choice.\n");
                }
            }
            else {
                printf("Invalid table.\n");
                actionChoice = 0;
            }
        }
    }

    printf("Bye.\n");
    return 0;
}
