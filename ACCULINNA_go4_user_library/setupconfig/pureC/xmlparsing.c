#include "xmlparsing.h"

#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

/* ================================================================================================ */
/* Global variables =============================================================================== */
/* ================================================================================================ */

enuTAG gCurTag;
stc_mapping gCurMappingInfo;

/* ================================================================================================ */
/* ================================================================================================ */
/* ================================================================================================ */

unsigned short CheckIfNextIsComment(FILE* f)
{
	/* char buffer[1024]; */
	unsigned short cursor;
	int readChar; /* Read char as int - return value of fgetc */

	unsigned short insideTag = 0;
	unsigned short insideComment = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		/* fprintf(stderr, "Checking: %c\n", (char)readChar); */
		/* buffer[cursor++] = readChar; /* FIXME nasty type cast! */
		cursor++; /* This is instead of the prev. commented line */
		if (readChar == ' ' || readChar == '\t' || readChar == '\n') {
			cursor--;
			/* Ignore whitespace between the tags or comments */
		}
		if (readChar == '<') {
			insideTag = 1;
			break;
		}
	}

	/* Look forward and check that the symbols are the expected !-- */
	/* In any case - return back to the position before the '<' sign using fseek */
	if (insideTag) {
		readChar = fgetc(f);
		if (readChar == '!') {
			readChar = fgetc(f);
			if (readChar == '-') {
				readChar = fgetc(f);
				if (readChar == '-') {
					insideComment = 1;
				} else {
					insideComment = 0;
				}
				/* long int curPos = ftell(f); */
				fseek(f, -4, SEEK_CUR);
				/* TODO check fseek return value */
			} else {
				/* long int curPos = ftell(f); */
				fseek(f, -3, SEEK_CUR);
				/* TODO check fseek return value */
			}
		} else {
			/* long int curPos = ftell(f); */
			fseek(f, -2, SEEK_CUR);
			/* TODO check fseek return value */
		}
	} else {
		/* long int curPos = ftell(f); */
		fseek(f, -1, SEEK_CUR);
		/* TODO check fseek return value */
	}

	return insideComment;
}

/* TODO now reading, should just skip */
unsigned short ReadComment(FILE* f)
{
	char buffer[10240]; /* TODO temporarily large buffer */
	unsigned short cursor;
	int readChar; /*  Read char as int - return value of fgetc */
	unsigned short endFound = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		buffer[cursor++] = readChar; /* FIXME nasty type cast! */
		if (readChar == '-') {
			readChar = fgetc(f);
			buffer[cursor++] = readChar; /* FIXME nasty type cast! */
			if (readChar == '-') {
				readChar = fgetc(f);
				buffer[cursor++] = readChar; /* FIXME nasty type cast! */
				if (readChar == '>') {
					endFound = 1;
					/*  buffer contains now the comment */
					/*  cursor has the size of the buffer */
					break;
				} else { }
			} else { }
		} else { }
	}
	/* Skip 3 traling symbols --> for printing */
	buffer[cursor/*-3*/] = '\0'; /* This trailing zero allows using printf */

	/* fprintf(stderr, "============================================================\n");
	   fprintf(stderr, "%s\n", buffer); /* (buffer+4) to skip first 4 symbols <!--
	   fprintf(stderr, "============================================================\n"); */

	return (endFound==1) ? cursor : 0;
}

unsigned short ReadTag(FILE* f, char* o_buffer, unsigned short* o_buffersize)
{
	unsigned short cursor;
	int readChar; /* Read char as int - return value of fgetc */
	unsigned short endFound = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		o_buffer[cursor++] = readChar; /* FIXME nasty type cast! */
		if (readChar == '>') {
			endFound = 1;
			/* o_buffer contains now the tag */
			/* cursor has the size of the o_buffer */
			break;
		}
	}
	/* Skip 1 traling symbol > for further parsing */
	o_buffer[cursor/*-1*/] = '\0'; /* This trailing zero allows using printf */
	/* fprintf(stderr, "|%s|\n", o_buffer); */

	*o_buffersize = cursor;

	return (endFound==1) ? cursor : 0;
}

void ProcessTag(stc_setup_config* ptr, char* o_buffer/*, enuTAG* o_CurTag*/)
{
	/* fprintf(stderr, "Processing:|%s|\n", o_buffer); */

	char* tok;
	size_t toklen;
	const char delims[] = " \t";

	/* One could init (i.e.) clear here - before processing the next tag */
	gCurTag = NOTAG; /* Current type of the tag being processed */

	ResetStcMapping(&gCurMappingInfo); /* Current mapping structure */

	tok = strtok(o_buffer, delims);

	/* TODO one could and one should implement many checks here */

	while (tok != NULL)
	{
		toklen = strlen(tok);
		/* fprintf(stderr, "size=%ld\n", toklen); */
		/* fprintf(stderr, "|%s|\n", tok); */

		/* Opening tag with no attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			/* fprintf(stderr, "opening no-attr tag: |%s|\n", &tok[1]); */

			if (strcmp(tok, "<setup") == 0) {
				gCurTag = SETUP;
			} else if (strcmp(tok, "<crate") == 0) {
				gCurTag = CRATE;
			} else if (strcmp(tok, "<mapping") == 0) {
				gCurTag = MAPPING;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				gCurTag = NOTAG;
				return;
			}
		}

		/* Opening tag with no attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-1] != '>')) {
			/* fprintf(stderr, "opening tag: |%s|\n", &tok[1]); */

			if (strcmp(tok, "<setup") == 0) {
				gCurTag = SETUP;
			} else if (strcmp(tok, "<crate") == 0) {
				gCurTag = CRATE;
			} else if (strcmp(tok, "<mapping") == 0) {
				gCurTag = MAPPING;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				gCurTag = NOTAG;
				return;
			}
		}

		/* Self-closing tag with no attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] == '/') && (tok[toklen-1] == '>')) {
			tok[toklen-2] = '\0';
			/* fprintf(stderr, "self-closing no-attr tag: |%s|\n", &tok[1]); */
			/* TODO what do we do here? */
		}

		/* Closing tag with no attributes */
		if ((tok[0] == '<') && (tok[1] == '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			/* fprintf(stderr, "closing no-attr tag: |%s|\n", &tok[2]); */

			if (strcmp(tok, "</setup") == 0) {
				gCurTag = NOTAG;
			} else if (strcmp(tok, "</crate") == 0) {
				/* This, basically, could be omitted.
				   We clear the full current mapping strcture when we get to the closing
				   crate tag. This includes clearing of the crate-related fields. */
				gCurTag = NOTAG; /* Current type of the tag being processed */
				InitStcMapping(&gCurMappingInfo); /* Current mapping structure */

				gCurTag = NOTAG;
			} else if (strcmp(tok, "</mapping") == 0) {
				/* There should be no such closing tag.
				   The <mapping/> tag is self-closing */
				gCurTag = NOTAG;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown closing tag %s.\n", &tok[2]);
				gCurTag = NOTAG;
				return;
			}

		}

		/* Not the last attribute */
		if ((tok[0] != '<') && (tok[toklen-1] != '>')) {
			/* fprintf(stderr, "attr: |%s|\n", tok); */

			char token2[1024];
			strncpy(token2, tok, 1024);
			ProcessAttr(ptr, token2/*, o_CurTag*/, 0);

		}
		/* Last attribute */
		if ((tok[0] != '<') && (tok[toklen-1] == '>')) {
			if (tok[toklen-2] == '/') {
				/* self-closing - trim 2 symbols */
				tok[toklen-2] = '\0';
			} else {
				/* not self-closing - trim 1 symbol */
				tok[toklen-1] = '\0';
			}

			/* fprintf(stderr, "last attr: |%s|\n", tok); */

			if (strlen(tok) > 0) {
				/* TODO костыль, решающий пробелы между последним аттрибутом и знаком '>' */
				char token2[1024];
				strncpy(token2, tok, 1024);
				ProcessAttr(ptr, token2/*, o_CurTag*/, 1);
			}
		}

		/* Go to the next token */
		tok = strtok (NULL, delims);
	} /* end of while over tokens */
}

void ProcessAttr(stc_setup_config* ptr, char* p_token/*, enuTAG* o_CurTag*/, unsigned short p_lastAttribute)
{
	/* Input token should look like */
	/* name="value" */

	size_t lastquotepos;
	size_t attnamelen;

	/* fprintf(stderr, "ProcessAttr: '%s'\n", p_token); */

	/* Search for the equal sign */
	char* equalsign = strchr(p_token, '=');

	/* If not found - error */
	if (equalsign == NULL) {
		/* ERROR */
		fprintf(stderr, "ERROR [ProcessAttr] attribute could not be parsed %s: no equal sign found.\n", p_token);
		fprintf(stderr, "      Most probably there is a space in a wrong place.\n");
		return;
	}

	/* Get the size of everything on the right from the equal sign */
	lastquotepos = strlen(equalsign)-1;
	/* Check that the 1) symbols after the equal sign and 2) the last symbol are the double-quote signs */
	/* If not - error */
	if ((equalsign[1] != '\"') || (equalsign[lastquotepos] != '\"')) {
		/* ERROR */
		fprintf(stderr, "ERROR [ProcessAttr] attribute could not be parsed %s:", p_token);
		fprintf(stderr, "no double-quotes found or there are spaces between the equal sign and the double-quotes.\n");
		return;
	}

	/* TODO WOW WOW WOW. Pure C... */
	/* Break the input token into the name and the value string */
	attnamelen = equalsign-p_token;
	p_token[attnamelen] = '\0';
	equalsign[lastquotepos] = '\0';

	/* fprintf(stderr, "%s = %s\n", p_token, &equalsign[2]); */

	/* Extract attributes' values and store them into the current (this) setup config object */
	if (gCurTag == SETUP) {
		if (strcmp(p_token, "name") == 0) {
			strncpy(ptr->fSetupName, &equalsign[2], 64);
		} else if (strcmp(p_token, "period") == 0) {
			ptr->fSetupPeriod = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "run") == 0) {
			ptr->fSetupRun = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "comment") == 0) {
			strncpy(ptr->fSetupComment, &equalsign[2], 128);
			/* fprintf(stderr, "fSetupComment=%s\n", ptr->fSetupComment); */
		} else {
			/* ERROR unknown attribute */
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}
	} else if (gCurTag == CRATE) {
		if (strcmp(p_token, "name") == 0) {
			/* strncpy(fCurCrateName, &equalsign[2], 64); */
			strncpy(gCurMappingInfo.fCrateName, &equalsign[2], 64);
		} else if (strcmp(p_token, "procid") == 0) {
			/* fCurCrateProcId = atoi(&equalsign[2]); */
			gCurMappingInfo.fCrateProcid = atoi(&equalsign[2]);
		} else {
			/* ERROR unknown attribute */
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}
	} else if (gCurTag == MAPPING) {
		if (strcmp(p_token, "addr") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			gCurMappingInfo.fAddr = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "elblock") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			strncpy(gCurMappingInfo.fElblock, &equalsign[2], 64);
		} else if (strcmp(p_token, "startelectrch") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			gCurMappingInfo.fStartelectrch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "nelectrch") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			gCurMappingInfo.fNelectrch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "stepelecrtch") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			gCurMappingInfo.fStepelecrtch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "detector") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			strncpy(gCurMappingInfo.fDetector, &equalsign[2], 64);
		} else if (strcmp(p_token, "startdetch") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			gCurMappingInfo.fStartdetch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "folder") == 0) {
			/* fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n", */
			/*         fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]); */
			strncpy(gCurMappingInfo.fFolder, &equalsign[2], 64);
		} else {
			/* ERROR unknown attribute */
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}

		if (p_lastAttribute == 1) {
			/*  Finish current MAPPING entity writeout */
			/*  DumpStcMapping(&gCurMappingInfo); */

			ExtendMappingsListStcSetupConfig(ptr, &gCurMappingInfo);

			/* TODO reset current MAPPING values */
		}
	}
}
