// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel

// Part of Microsoft CRT

int _chdir(char* path);
int _chdrive(int drive);
char* _getcwd(char* buffer, size_t length);
char* _getwd(char* path_name);
int _getdrive(void);
int _mkdir(const char* pathname);
int _rmdir(const char* pathname);
