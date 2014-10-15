#include <string.h>
#include <libIP2Location/IP2Location.h>

static void print_usage(const char *argv0)
{
    printf(
"ip2location --datafile [IP2LOCATION BIN DATA PATH] --ip [IP ADDRESS] [OPTIONS]\n"
"   -d, --datafile\n"
"        Specify the path of IP2Location .BIN data file.\n"
"\n"
"   -i, --inputfile\n"
"        Specify an input file with IP address list.\n"
"\n"
"   -p, --ip         \n"
"        Specify an IP address query (Supported IPv4 and IPv6 address).\n"
"\n"
"   -o, --outputfile \n"
"        Specify output file for query results.\n"
"\n"
"   -f, --format     \n"
"        Specify output format. Supported formats are:\n"
"            - CSV (default)\n"
"            - TAB\n"
"            - XML\n"
"               \n"
"   -h, -?, --help       \n"
"        Display this help file\n"
"\n"
"   -v, --version    \n"
"        Display the version number\n"
"   \n"
"   -e, --field          \n"
"        Specify the field to be displayed. Supported values are:\n"
"            - ip\n"
"            - countryshort\n"
"            - countrylong\n"
"            - region\n"
"            - city\n"
"            - isp\n"
"            - latitude\n"
"            - longitude\n"
"            - domain\n"
"            - zipcode\n"
"            - timezone\n"
"            - netspeed\n"
"            - iddcode\n"
"            - areacode\n"
"            - weatherstationcode\n"
"            - weatherstationname\n"
"            - mcc\n"
"            - mnc\n"
"            - mobilebrand\n"
"            - elevation\n"
"            - usagetype\n"
"            \n"
"   -n, --noheading              \n"
"        Suppress the heading display.\n");
}

static void print_version()
{
    printf("ip2location version 7.0.0\n");
}

static void print_footer(FILE *fout, const char *field, const char *format)
{
    if (strcmp(format, "XML") == 0) {
        fprintf(fout, "</xml>\n");
        return;
    }
}

static void print_header(FILE *fout, const char *field, const char *format)
{
    const char *start = field;
    const char *end = strchr(start, ',');
    int first = 1;

    if (strcmp(format, "XML") == 0) {
        fprintf(fout, "<xml>\n");
        return;
    }

#define WRITE_HEADER(field_name)  \
        if (strncmp(start, field_name, end - start) == 0) { \
            if (strcmp(format, "CSV") == 0) { \
                if (!first) { \
                    fprintf(fout, ","); \
                } \
                fprintf(fout, "\"%s\"", field_name); \
            } else if (strcmp(format, "TAB") == 0) { \
                if (!first) { \
                    fprintf(fout, "\t"); \
                } \
                fprintf(fout, "%s", field_name); \
            } \
            first = 0; \
        }
    for (;;) {
        if (end == NULL) {
            end = start + strlen(start);
        }

        WRITE_HEADER("ip");
        WRITE_HEADER("countryshort");
        WRITE_HEADER("countrylong");
        WRITE_HEADER("region");
        WRITE_HEADER("city");
        WRITE_HEADER("isp");
        WRITE_HEADER("latitude");
        WRITE_HEADER("longitude");
        WRITE_HEADER("domain");
        WRITE_HEADER("zipcode");
        WRITE_HEADER("timezone");
        WRITE_HEADER("netspeed");
        WRITE_HEADER("iddcode");
        WRITE_HEADER("areacode");
        WRITE_HEADER("weatherstationcode");
        WRITE_HEADER("weatherstationname");
        WRITE_HEADER("mcc");
        WRITE_HEADER("mnc");
        WRITE_HEADER("mobilebrand");
        WRITE_HEADER("elevation");
        WRITE_HEADER("usagetype");

        if (*end == ',') {
            start = end + 1;
            end = strchr(start, ',');
        } else {
            break;
        }
    }
    fprintf(fout, "\n");
}


static void print_record(FILE *fout, const char *field, IP2LocationRecord *record, const char *format, const char *ip)
{
    const char *start = field;
    const char *end = strchr(start, ',');
    int first = 1;

    if (strcmp(format, "XML") == 0) {
        fprintf(fout, "<row>");
    }

#define WRITE_FIELD(field_name, field)  \
        if (strncmp(start, field_name, end - start) == 0) { \
            const char *value = field; \
            if (strcmp(value, NOT_SUPPORTED) == 0) { \
                value = "N/A"; \
            } \
            if (strcmp(format, "XML") == 0) { \
                fprintf(fout, "<%s>%s</%s>", field_name, value, field_name); \
            } else if (strcmp(format, "CSV") == 0) { \
                if (!first) { \
                    fprintf(fout, ","); \
                } \
                fprintf(fout, "\"%s\"", value); \
            } else if (strcmp(format, "TAB") == 0) { \
                if (!first) { \
                    fprintf(fout, "\t"); \
                } \
                fprintf(fout, "%s", value); \
            } \
            first = 0; \
        }
#define WRITE_FIELDF(field_name, field)  \
        if (strncmp(start, field_name, end - start) == 0) { \
            if (strcmp(format, "XML") == 0) { \
                fprintf(fout, "<%s>%f</%s>", field_name, field, field_name); \
            } else if (strcmp(format, "CSV") == 0) { \
                if (!first) { \
                    fprintf(fout, ","); \
                } \
                fprintf(fout, "\"%f\"", field); \
            } else if (strcmp(format, "TAB") == 0) { \
                if (!first) { \
                    fprintf(fout, "\t"); \
                } \
                fprintf(fout, "%f", field); \
            } \
            first = 0; \
        }


    for (;;) {
        if (end == NULL) {
            end = start + strlen(start);
        }

        WRITE_FIELD("ip", ip);
        WRITE_FIELD("countryshort", record->country_short);
        WRITE_FIELD("countrylong", record->country_long);
        WRITE_FIELD("region", record->region);
        WRITE_FIELD("city", record->city);
        WRITE_FIELD("isp", record->isp);
        WRITE_FIELDF("latitude", record->latitude);
        WRITE_FIELDF("longitude", record->longitude);
        WRITE_FIELD("domain", record->domain);
        WRITE_FIELD("zipcode", record->zipcode);
        WRITE_FIELD("timezone", record->timezone);
        WRITE_FIELD("netspeed", record->netspeed);
        WRITE_FIELD("iddcode", record->iddcode);
        WRITE_FIELD("areacode", record->areacode);
        WRITE_FIELD("weatherstationcode", record->weatherstationcode);
        WRITE_FIELD("weatherstationname", record->weatherstationname);
        WRITE_FIELD("mcc", record->mcc);
        WRITE_FIELD("mnc", record->mnc);
        WRITE_FIELD("mobilebrand", record->mobilebrand);
        WRITE_FIELDF("elevation", record->elevation);
        WRITE_FIELD("usagetype", record->usagetype);

        if (*end == ',') {
            start = end + 1;
            end = strchr(start, ',');
        } else {
            break;
        }
    }
    if (strcmp(format, "XML") == 0) {
        fprintf(fout, "</row>");
    }
    fprintf(fout, "\n");
}

int main(int argc, char *argv[])
{
    int i;
    const char *datafile = NULL;
    const char *inputfile = NULL;
    const char *ip = NULL;
    const char *outputfile = NULL;
    const char *format = "CSV";
    const char *field = NULL;
    int noheading = 0;
    IP2Location *dbobj = NULL;
    IP2LocationRecord *record = NULL;
    FILE *fout = stdout;

    field = "ip,countryshort,countrylong,region,city,isp,latitude,longitude,domain,zipcode,timezone,netspeed,iddcode,areacode,weatherstationcode,weatherstationname,mcc,mnc,mobilebrand,elevation,usagetype";

    for (i = 1; i < argc; i++) {
        const char *argvi = argv[i];

        if (strcmp(argvi, "-d") == 0 || strcmp(argvi, "--datafile") == 0) {
            if (i + 1 < argc) {
                datafile = argv[++i];
            }
        } else if (strcmp(argvi, "-i") == 0 || strcmp(argvi, "--inputfile") == 0) {
            if (i + 1 < argc) {
                inputfile = argv[++i];
            }
        } else if (strcmp(argvi, "-p") == 0 || strcmp(argvi, "--ip") == 0) {
            if (i + 1 < argc) {
                ip = argv[++i];
            }
        } else if (strcmp(argvi, "-o") == 0 || strcmp(argvi, "--outputfile") == 0) {
            if (i + 1 < argc) {
                outputfile = argv[++i];
            }
        } else if (strcmp(argvi, "-f") == 0 || strcmp(argvi, "--format") == 0) {
            if (i + 1 < argc) {
                format = argv[++i];
            }
        } else if (strcmp(argvi, "-h") == 0 || strcmp(argvi, "-?") == 0 || strcmp(argvi, "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argvi, "-v") == 0 || strcmp(argvi, "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argvi, "-e") == 0 || strcmp(argvi, "--field") == 0) {
            if (i + 1 < argc) {
                field = argv[++i];
            }
        } else if (strcmp(argvi, "-n") == 0 || strcmp(argvi, "--noheading") == 0) {
            noheading = 1;
        }
    }

    if (strcmp(format, "CSV") != 0 && strcmp(format, "XML") != 0 && strcmp(format, "TAB") != 0) {
        fprintf(stderr, "Invalid format %s, supported formats: CSV, XML, TAB\n", format);
        exit(-1);
    }

    if (datafile == NULL) {
        fprintf(stderr, "Datafile is absent\n");
        exit(-1);
    }

	if (ip == NULL) {
        fprintf(stderr, "IP address is absent\n");
        exit(-1);
    }
	
    dbobj = IP2Location_open((char *)datafile);
    if (dbobj == NULL) {
        fprintf(stderr, "Failed to open datafile %s\n", datafile);
        exit(-1);
    }

    if (outputfile != NULL) {
        fout = fopen(outputfile, "w");
        if (fout == NULL) {
            fprintf(stderr, "Failed to open outputfile %s\n", outputfile);
            exit(-1);
        }
    }

    if (!noheading) {
        print_header(fout, field, format);
    }

    if (ip != NULL) {
        record = IP2Location_get_all(dbobj, (char *)ip);
        print_record(fout, field, record, format, ip);
        IP2Location_free_record(record);
    }
    if (inputfile != NULL) {
        char *line = NULL;
        size_t n;
        ssize_t len;
        FILE *fin = fopen(inputfile, "r");

        if (fin == NULL) {
            fprintf(stderr, "Failed to open inputfile %s\n", inputfile);
            exit(-1);
        }

        while ((len = getline(&line, &n, fin)) != -1) {
            if (line[len - 1] == '\n') {
                line[--len] = '\0';
            }
            if (line[len - 1] == '\r') {
                line[--len] = '\0';
            }
            record = IP2Location_get_all(dbobj, line);
            print_record(fout, field, record, format, line);
            IP2Location_free_record(record);
        }

        fclose(fin);
    }
    print_footer(fout, field, format);
    IP2Location_close(dbobj);
    // OS will clean-up the rest
    return 0;
}
