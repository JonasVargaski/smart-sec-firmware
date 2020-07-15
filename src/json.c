#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getIntJSON(char buff[], char key[])
{
  int i, j = 0;
  char aux[10] = "";
  char attr[20] = "";
  char expected[20] = "";
  char find = 0;
  int value = -1;

  sprintf(expected, "\"%s\":", key);

  for (i = 0; i < strlen(buff) && value == -1; i++)
  {
    if (buff[i] == expected[j] && !find)
    {
      sprintf(attr, "%s%c", attr, buff[i]);
      j++;
      if (strstr(attr, expected))
      {
        j = 0;
        find = 1;
      }
    }
    else if (!find)
    {
      j = 0;
      sprintf(attr, "");
    }
    else
    {
      if (buff[i] == '"' || buff[i] == ',' || buff[i] == '}')
      {
        value = atoi(aux);
      }
      else
      {
        aux[j++] = buff[i];
      }
    }
  }
  return value;
}

void getStringJSON(char buff[], char key[], char *ret)
{
  unsigned int i, j = 0;
  char res[25] = "";
  char attr[20] = "";
  char expected[20] = "";
  char find = 0;
  char isValid = 0;

  sprintf(expected, "\"%s\":\"", key);

  for (i = 0; i < strlen(buff) && isValid == 0; i++)
  {
    if (buff[i] == expected[j] && !find)
    {
      sprintf(attr, "%s%c", attr, buff[i]);
      j++;
      if (strstr(attr, expected))
      {
        j = 0;
        find = 1;
      }
    }
    else if (!find)
    {
      j = 0;
      sprintf(attr, "");
    }
    else
    {
      if (buff[i] == '"')
      {
        isValid = 1;
        sprintf(ret, "%s", res);
      }
      else
      {
        sprintf(res, "%s%c", res, buff[i]);
      }
    }
  }
}