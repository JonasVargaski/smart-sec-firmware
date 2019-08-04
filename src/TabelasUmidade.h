#ifndef TABELASUMIDADE_H
#define	TABELASUMIDADE_H

unsigned int tabelaUmidade(int _tmp, int _clima, char _tipo_sensor) {
    char _umd = 0;
    if (_tipo_sensor == 0) {
        switch (_clima) {
            case 1:// Umido
                if (_tmp <= 90) _umd = 85;
                else
                    if (_tmp == 91) _umd = 86;
                else
                    if (_tmp == 92) _umd = 87;
                else
                    if (_tmp == 93) _umd = 88;
                else
                    if (_tmp == 94) _umd = 89;
                else
                    if (_tmp == 95) _umd = 90;
                else
                    if (_tmp == 96) _umd = 91;
                else
                    if (_tmp == 97) _umd = 92;
                else
                    if (_tmp == 98) _umd = 93;
                else
                    if (_tmp == 99) _umd = 94;
                else
                    if (_tmp == 100)_umd = 95;
                else
                    if (_tmp == 101)_umd = 94;
                else
                    if (_tmp >= 102 && _tmp <= 107) _umd = 94;
                else
                    if (_tmp >= 108 && _tmp <= 118) _umd = 95;
                else
                    if (_tmp == 119 | _tmp == 120) _umd = 96;
                else
                    if (_tmp == 121 | _tmp == 122) _umd = 97;
                else
                    if (_tmp == 123 | _tmp == 124) _umd = 98;
                else
                    if (_tmp == 125 | _tmp == 126) _umd = 99;
                else
                    if (_tmp == 127 | _tmp == 128) _umd = 100;
                else
                    if (_tmp == 129 | _tmp == 130) _umd = 101;
                else
                    if (_tmp == 131 | _tmp == 132) _umd = 102;
                else
                    if (_tmp == 133 | _tmp == 134) _umd = 103;
                else
                    if (_tmp >= 135 && _tmp <= 146) _umd = 104;
                else
                    if (_tmp >= 147 && _tmp <= 150) _umd = 105;
                else
                    if (_tmp == 151 | _tmp == 152) _umd = 106;
                else
                    if (_tmp == 153) _umd = 107;
                else
                    if (_tmp == 154 | _tmp == 155) _umd = 108;
                else
                    if (_tmp == 156 | _tmp == 157) _umd = 109;
                else
                    if (_tmp == 158 | _tmp == 159) _umd = 110;
                else
                    if (_tmp >= 160) _umd = 112;
                break;

            case 2: //Normal
                if (_tmp <= 90) _umd = 88;
                else
                    if (_tmp == 91) _umd = 89;
                else
                    if (_tmp == 92) _umd = 90;
                else
                    if (_tmp == 93) _umd = 91;
                else
                    if (_tmp == 94) _umd = 92;
                else
                    if (_tmp == 95) _umd = 93;
                else
                    if (_tmp == 96) _umd = 94;
                else
                    if (_tmp == 97) _umd = 95;
                else
                    if (_tmp >= 98 && _tmp <= 100) _umd = 96;
                else
                    if (_tmp >= 101 && _tmp <= 119)_umd = 97;
                else
                    if (_tmp >= 120 && _tmp <= 122)_umd = 98;
                else
                    if (_tmp == 123 | _tmp == 124) _umd = 99;
                else
                    if (_tmp == 125 | _tmp == 126) _umd = 100;
                else
                    if (_tmp == 127 | _tmp == 128) _umd = 101;
                else
                    if (_tmp == 129 | _tmp == 130) _umd = 102;
                else
                    if (_tmp == 131 | _tmp == 132) _umd = 103;
                else
                    if (_tmp >= 133 && _tmp <= 148)_umd = 104;
                else
                    if (_tmp >= 149 && _tmp <= 152)_umd = 105;
                else
                    if (_tmp == 153) _umd = 106;
                else
                    if (_tmp == 154) _umd = 107;
                else
                    if (_tmp == 155 | _tmp == 156) _umd = 108;
                else
                    if (_tmp == 157 | _tmp == 158) _umd = 109;
                else
                    if (_tmp == 159) _umd = 110;
                else
                    if (_tmp >= 160) _umd = 112;
                break;

            case 3: // Seco
                if (_tmp <= 90) _umd = 90;
                else
                    if (_tmp == 91) _umd = 91;
                else
                    if (_tmp == 92) _umd = 92;
                else
                    if (_tmp == 93) _umd = 93;
                else
                    if (_tmp == 94) _umd = 94;
                else
                    if (_tmp == 95) _umd = 95;
                else
                    if (_tmp == 96) _umd = 95;
                else
                    if (_tmp == 97) _umd = 96;
                else
                    if (_tmp == 98) _umd = 96;
                else
                    if (_tmp == 99) _umd = 97;
                else
                    if (_tmp >= 100 && _tmp <= 103) _umd = 98;
                else
                    if (_tmp == 104 | _tmp == 105) _umd = 99;
                else
                    if (_tmp >= 106 && _tmp <= 122) _umd = 100;
                else
                    if (_tmp == 123 | _tmp == 124) _umd = 101;
                else
                    if (_tmp == 125 | _tmp == 126) _umd = 102;
                else
                    if (_tmp == 127 | _tmp == 128) _umd = 103;
                else
                    if (_tmp >= 129 && _tmp <= 138) _umd = 105;
                else
                    if (_tmp == 139 | _tmp == 140) _umd = 106;
                else
                    if (_tmp == 141 | _tmp == 146) _umd = 107;
                else
                    if (_tmp >= 147 && _tmp <= 151) _umd = 108;
                else
                    if (_tmp == 153 | _tmp == 152) _umd = 109;
                else
                    if (_tmp == 155 | _tmp == 154) _umd = 111;
                else
                    if (_tmp == 157 | _tmp == 156) _umd = 112;
                else
                    if (_tmp == 159 | _tmp == 158) _umd = 113;
                else
                    if (_tmp >= 160) _umd = 115;
                break;
        }
    } else if (_tipo_sensor == 1) {
        switch (clima) {
            case 1:// Umido
                if (_tmp <= 90) _umd = 92;
                else
                    if (_tmp == 91) _umd = 92;
                else
                    if (_tmp == 92) _umd = 90;
                else
                    if (_tmp == 93) _umd = 89;
                else
                    if (_tmp >= 94 && _tmp <= 100) _umd = 88;
                else
                    if (_tmp >= 101 && _tmp <= 102) _umd = 78;
                else
                    if (_tmp == 103) _umd = 76;
                else
                    if (_tmp == 104) _umd = 75;
                else
                    if (_tmp == 105) _umd = 72;
                else
                    if (_tmp == 106) _umd = 70;
                else
                    if (_tmp == 107) _umd = 68;
                else
                    if (_tmp == 108) _umd = 67;
                else
                    if (_tmp == 109) _umd = 66;
                else
                    if (_tmp == 110) _umd = 65;
                else
                    if (_tmp >= 111 && _tmp <= 114) _umd = 60;
                else
                    if (_tmp == 115) _umd = 58;
                else
                    if (_tmp == 116) _umd = 56;
                else
                    if (_tmp == 117) _umd = 54;
                else
                    if (_tmp == 118) _umd = 52;
                else
                    if (_tmp == 119) _umd = 50;
                else
                    if (_tmp == 120) _umd = 48;
                else
                    if (_tmp == 121) _umd = 47;
                else
                    if (_tmp == 122) _umd = 46;
                else
                    if (_tmp == 123) _umd = 45;
                else
                    if (_tmp == 124) _umd = 44;
                else
                    if (_tmp == 125) _umd = 43;
                else
                    if (_tmp == 126) _umd = 42;
                else
                    if (_tmp == 127) _umd = 40;
                else
                    if (_tmp == 128) _umd = 40;
                else
                    if (_tmp == 129) _umd = 39;
                else
                    if (_tmp == 130) _umd = 38;
                else
                    if (_tmp == 131) _umd = 37;
                else
                    if (_tmp == 132) _umd = 36;
                else
                    if (_tmp == 133) _umd = 36;
                else
                    if (_tmp == 134) _umd = 36;
                else
                    if (_tmp == 135) _umd = 35;
                else
                    if (_tmp == 136) _umd = 34;
                else
                    if (_tmp == 137) _umd = 33;
                else
                    if (_tmp == 138) _umd = 32;
                else
                    if (_tmp == 139) _umd = 31;
                else
                    if (_tmp == 140) _umd = 30;
                else
                    if (_tmp == 141) _umd = 29;
                else
                    if (_tmp == 142) _umd = 28;
                else
                    if (_tmp == 143) _umd = 27;
                else
                    if (_tmp == 144) _umd = 26;
                else
                    if (_tmp == 145) _umd = 25;
                else
                    if (_tmp == 146) _umd = 24;
                else
                    if (_tmp == 147) _umd = 23;
                else
                    if (_tmp == 148) _umd = 22;
                else
                    if (_tmp == 149) _umd = 21;
                else
                    if (_tmp == 150) _umd = 20;
                else
                    if (_tmp == 151) _umd = 19;
                else
                    if (_tmp == 152) _umd = 18;
                else
                    if (_tmp >= 153 && _tmp <= 156) _umd = 17;
                else
                    if (_tmp == 157) _umd = 16;
                else
                    if (_tmp >= 158) _umd = 15;
                break;
            case 2:// Normal
                if (_tmp <= 90) _umd = 97;
                else
                    if (_tmp == 91) _umd = 96;
                else
                    if (_tmp == 92) _umd = 95;
                else
                    if (_tmp == 92) _umd = 95;
                else
                    if (_tmp == 93) _umd = 94;
                else
                    if (_tmp >= 94 && _tmp <= 100) _umd = 93;
                else
                    if (_tmp >= 101 && _tmp <= 102) _umd = 83;
                else
                    if (_tmp == 103) _umd = 81;
                else
                    if (_tmp == 104) _umd = 80;
                else
                    if (_tmp == 105) _umd = 77;
                else
                    if (_tmp == 106) _umd = 75;
                else
                    if (_tmp == 107) _umd = 73;
                else
                    if (_tmp == 108) _umd = 72;
                else
                    if (_tmp == 109) _umd = 71;
                else
                    if (_tmp == 110) _umd = 70;
                else
                    if (_tmp >= 111 && _tmp <= 114) _umd = 65;
                else
                    if (_tmp == 115) _umd = 63;
                else
                    if (_tmp == 116) _umd = 61;
                else
                    if (_tmp == 117) _umd = 59;
                else
                    if (_tmp == 118) _umd = 57;
                else
                    if (_tmp == 119) _umd = 55;
                else
                    if (_tmp == 120) _umd = 53;
                else
                    if (_tmp == 121) _umd = 51;
                else
                    if (_tmp == 122) _umd = 50;
                else
                    if (_tmp == 123) _umd = 48;
                else
                    if (_tmp == 124) _umd = 47;
                else
                    if (_tmp == 125) _umd = 45;
                else
                    if (_tmp == 126) _umd = 44;
                else
                    if (_tmp == 127) _umd = 42;
                else
                    if (_tmp == 128) _umd = 41;
                else
                    if (_tmp == 129) _umd = 41;
                else
                    if (_tmp == 130) _umd = 40;
                else
                    if (_tmp == 131) _umd = 40;
                else
                    if (_tmp == 132) _umd = 39;
                else
                    if (_tmp == 133) _umd = 39;
                else
                    if (_tmp == 134) _umd = 36;
                else
                    if (_tmp == 135) _umd = 35;
                else
                    if (_tmp == 136) _umd = 34;
                else
                    if (_tmp == 137) _umd = 33;
                else
                    if (_tmp == 138) _umd = 32;
                else
                    if (_tmp == 139) _umd = 31;
                else
                    if (_tmp == 140) _umd = 30;
                else
                    if (_tmp == 141) _umd = 29;
                else
                    if (_tmp == 142) _umd = 28;
                else
                    if (_tmp == 143) _umd = 27;
                else
                    if (_tmp == 144) _umd = 26;
                else
                    if (_tmp == 145) _umd = 25;
                else
                    if (_tmp == 146) _umd = 24;
                else
                    if (_tmp == 147) _umd = 23;
                else
                    if (_tmp == 148) _umd = 22;
                else
                    if (_tmp == 149) _umd = 21;
                else
                    if (_tmp == 150) _umd = 20;
                else
                    if (_tmp == 151) _umd = 19;
                else
                    if (_tmp == 152) _umd = 18;
                else
                    if (_tmp >= 153 && _tmp <= 156) _umd = 17;
                else
                    if (_tmp == 157) _umd = 16;
                else
                    if (_tmp >= 158) _umd = 15;
                break;
            case 3:// Seco
                if (_tmp <= 92) _umd = 100;
                else
                    if (_tmp == 93) _umd = 99;
                else
                    if (_tmp >= 94 && _tmp <= 100) _umd = 98;
                else
                    if (_tmp >= 101 && _tmp <= 102) _umd = 88;
                else
                    if (_tmp == 103) _umd = 86;
                else
                    if (_tmp == 104) _umd = 85;
                else
                    if (_tmp == 105) _umd = 82;
                else
                    if (_tmp == 106) _umd = 80;
                else
                    if (_tmp == 107) _umd = 78;
                else
                    if (_tmp == 108) _umd = 77;
                else
                    if (_tmp == 109) _umd = 76;
                else
                    if (_tmp == 110) _umd = 75;
                else
                    if (_tmp >= 111 && _tmp <= 114) _umd = 70;
                else
                    if (_tmp == 115) _umd = 68;
                else
                    if (_tmp == 116) _umd = 66;
                else
                    if (_tmp == 117) _umd = 64;
                else
                    if (_tmp == 118) _umd = 62;
                else
                    if (_tmp == 119) _umd = 60;
                else
                    if (_tmp == 120) _umd = 58;
                else
                    if (_tmp == 121) _umd = 56;
                else
                    if (_tmp == 122) _umd = 55;
                else
                    if (_tmp == 123) _umd = 53;
                else
                    if (_tmp == 124) _umd = 52;
                else
                    if (_tmp == 125) _umd = 50;
                else
                    if (_tmp == 126) _umd = 49;
                else
                    if (_tmp == 127) _umd = 47;
                else
                    if (_tmp == 128) _umd = 45;
                else
                    if (_tmp == 129) _umd = 43;
                else
                    if (_tmp == 130) _umd = 41;
                else
                    if (_tmp == 131) _umd = 40;
                else
                    if (_tmp == 132) _umd = 39;
                else
                    if (_tmp == 133) _umd = 37;
                else
                    if (_tmp == 134) _umd = 36;
                else
                    if (_tmp == 135) _umd = 35;
                else
                    if (_tmp == 136) _umd = 34;
                else
                    if (_tmp == 137) _umd = 33;
                else
                    if (_tmp == 138) _umd = 32;
                else
                    if (_tmp == 139) _umd = 31;
                else
                    if (_tmp == 140) _umd = 30;
                else
                    if (_tmp == 141) _umd = 29;
                else
                    if (_tmp == 142) _umd = 28;
                else
                    if (_tmp == 143) _umd = 27;
                else
                    if (_tmp == 144) _umd = 26;
                else
                    if (_tmp == 145) _umd = 25;
                else
                    if (_tmp == 146) _umd = 24;
                else
                    if (_tmp == 147) _umd = 23;
                else
                    if (_tmp == 148) _umd = 22;
                else
                    if (_tmp == 149) _umd = 21;
                else
                    if (_tmp == 150) _umd = 20;
                else
                    if (_tmp == 151) _umd = 19;
                else
                    if (_tmp == 152) _umd = 18;
                else
                    if (_tmp >= 153 && _tmp <= 156) _umd = 17;
                else
                    if (_tmp == 157) _umd = 16;
                else
                    if (_tmp >= 158) _umd = 15;
                break;
        }
    }
    return _umd;
}













#endif	/* XC_HEADER_TEMPLATE_H */

