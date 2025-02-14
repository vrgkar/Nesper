#include "DebugWindow.h"

#include <random>

DebugWindow::DebugWindow(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Debugger", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    richTextCtrl = new wxRichTextCtrl(this, wxID_ANY, "",
                                                  wxDefaultPosition, wxDefaultSize,
                                                  wxTE_MULTILINE | wxTE_READONLY);  

    wxRichTextAttr attr;
        attr.SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));  // Set a monospace font



    richTextCtrl->SetDefaultStyle(attr);
    wxButton *button_next = new wxButton(this, wxID_ANY, "Next");
    richTextCtrl->AppendText(
        R"(C000  4C F5 C5  JMP $C5F5                       A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 21 CYC:7
C5F5  A2 00     LDX #$00                        A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 30 CYC:10
C5F7  86 00     STX $00 = 00                    A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 36 CYC:12
C5F9  86 10     STX $10 = 00                    A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 45 CYC:15
C5FB  86 11     STX $11 = 00                    A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 54 CYC:18
C5FD  20 2D C7  JSR $C72D                       A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 63 CYC:21
C72D  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0, 81 CYC:27
C72E  38        SEC                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0, 87 CYC:29
C72F  B0 04     BCS $C735                       A:00 X:00 Y:00 P:27 SP:FB PPU:  0, 93 CYC:31
C735  EA        NOP                             A:00 X:00 Y:00 P:27 SP:FB PPU:  0,102 CYC:34
C736  18        CLC                             A:00 X:00 Y:00 P:27 SP:FB PPU:  0,108 CYC:36
C737  B0 03     BCS $C73C                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,114 CYC:38
C739  4C 40 C7  JMP $C740                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,120 CYC:40
C740  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0,129 CYC:43
C741  38        SEC                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0,135 CYC:45
C742  90 03     BCC $C747                       A:00 X:00 Y:00 P:27 SP:FB PPU:  0,141 CYC:47
C744  4C 4B C7  JMP $C74B                       A:00 X:00 Y:00 P:27 SP:FB PPU:  0,147 CYC:49
C74B  EA        NOP                             A:00 X:00 Y:00 P:27 SP:FB PPU:  0,156 CYC:52
C74C  18        CLC                             A:00 X:00 Y:00 P:27 SP:FB PPU:  0,162 CYC:54
C74D  90 04     BCC $C753                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,168 CYC:56
C753  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0,177 CYC:59
C754  A9 00     LDA #$00                        A:00 X:00 Y:00 P:26 SP:FB PPU:  0,183 CYC:61
C756  F0 04     BEQ $C75C                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,189 CYC:63
C75C  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0,198 CYC:66
C75D  A9 40     LDA #$40                        A:00 X:00 Y:00 P:26 SP:FB PPU:  0,204 CYC:68
C75F  F0 03     BEQ $C764                       A:40 X:00 Y:00 P:24 SP:FB PPU:  0,210 CYC:70
C761  4C 68 C7  JMP $C768                       A:40 X:00 Y:00 P:24 SP:FB PPU:  0,216 CYC:72
C768  EA        NOP                             A:40 X:00 Y:00 P:24 SP:FB PPU:  0,225 CYC:75
C769  A9 40     LDA #$40                        A:40 X:00 Y:00 P:24 SP:FB PPU:  0,231 CYC:77
C76B  D0 04     BNE $C771                       A:40 X:00 Y:00 P:24 SP:FB PPU:  0,237 CYC:79
C771  EA        NOP                             A:40 X:00 Y:00 P:24 SP:FB PPU:  0,246 CYC:82
C772  A9 00     LDA #$00                        A:40 X:00 Y:00 P:24 SP:FB PPU:  0,252 CYC:84
C774  D0 03     BNE $C779                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,258 CYC:86
C776  4C 7D C7  JMP $C77D                       A:00 X:00 Y:00 P:26 SP:FB PPU:  0,264 CYC:88
C77D  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  0,273 CYC:91
C77E  A9 FF     LDA #$FF                        A:00 X:00 Y:00 P:26 SP:FB PPU:  0,279 CYC:93
C780  85 01     STA $01 = 00                    A:FF X:00 Y:00 P:A4 SP:FB PPU:  0,285 CYC:95
C782  24 01     BIT $01 = FF                    A:FF X:00 Y:00 P:A4 SP:FB PPU:  0,294 CYC:98
C784  70 04     BVS $C78A                       A:FF X:00 Y:00 P:E4 SP:FB PPU:  0,303 CYC:101
C78A  EA        NOP                             A:FF X:00 Y:00 P:E4 SP:FB PPU:  0,312 CYC:104
C78B  24 01     BIT $01 = FF                    A:FF X:00 Y:00 P:E4 SP:FB PPU:  0,318 CYC:106
C78D  50 03     BVC $C792                       A:FF X:00 Y:00 P:E4 SP:FB PPU:  0,327 CYC:109
C78F  4C 96 C7  JMP $C796                       A:FF X:00 Y:00 P:E4 SP:FB PPU:  0,333 CYC:111
C796  EA        NOP                             A:FF X:00 Y:00 P:E4 SP:FB PPU:  1,  1 CYC:114
C797  A9 00     LDA #$00                        A:FF X:00 Y:00 P:E4 SP:FB PPU:  1,  7 CYC:116
C799  85 01     STA $01 = FF                    A:00 X:00 Y:00 P:66 SP:FB PPU:  1, 13 CYC:118
C79B  24 01     BIT $01 = 00                    A:00 X:00 Y:00 P:66 SP:FB PPU:  1, 22 CYC:121
C79D  50 04     BVC $C7A3                       A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 31 CYC:124
C7A3  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 40 CYC:127
C7A4  24 01     BIT $01 = 00                    A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 46 CYC:129
C7A6  70 03     BVS $C7AB                       A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 55 CYC:132
C7A8  4C AF C7  JMP $C7AF                       A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 61 CYC:134
C7AF  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 70 CYC:137
C7B0  A9 00     LDA #$00                        A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 76 CYC:139
C7B2  10 04     BPL $C7B8                       A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 82 CYC:141
C7B8  EA        NOP                             A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 91 CYC:144
C7B9  A9 80     LDA #$80                        A:00 X:00 Y:00 P:26 SP:FB PPU:  1, 97 CYC:146
C7BB  10 03     BPL $C7C0                       A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,103 CYC:148
C7BD  4C D9 C7  JMP $C7D9                       A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,109 CYC:150
C7D9  EA        NOP                             A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,118 CYC:153
C7DA  60        RTS                             A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,124 CYC:155
C600  20 DB C7  JSR $C7DB                       A:80 X:00 Y:00 P:A4 SP:FD PPU:  1,142 CYC:161
C7DB  EA        NOP                             A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,160 CYC:167
C7DC  A9 FF     LDA #$FF                        A:80 X:00 Y:00 P:A4 SP:FB PPU:  1,166 CYC:169
C7DE  85 01     STA $01 = 00                    A:FF X:00 Y:00 P:A4 SP:FB PPU:  1,172 CYC:171
C7E0  24 01     BIT $01 = FF                    A:FF X:00 Y:00 P:A4 SP:FB PPU:  1,181 CYC:174
C7E2  A9 00     LDA #$00                        A:FF X:00 Y:00 P:E4 SP:FB PPU:  1,190 CYC:177
C7E4  38        SEC                             A:00 X:00 Y:00 P:66 SP:FB PPU:  1,196 CYC:179
C7E5  78        SEI                             A:00 X:00 Y:00 P:67 SP:FB PPU:  1,202 CYC:181
C7E6  F8        SED                             A:00 X:00 Y:00 P:67 SP:FB PPU:  1,208 CYC:183
C7E7  08        PHP                             A:00 X:00 Y:00 P:6F SP:FB PPU:  1,214 CYC:185
C7E8  68        PLA                             A:00 X:00 Y:00 P:6F SP:FA PPU:  1,223 CYC:188
C7E9  29 EF     AND #$EF                        A:7F X:00 Y:00 P:6D SP:FB PPU:  1,235 CYC:192
C7EB  C9 6F     CMP #$6F                        A:6F X:00 Y:00 P:6D SP:FB PPU:  1,241 CYC:194
C7ED  F0 04     BEQ $C7F3                       A:6F X:00 Y:00 P:6F SP:FB PPU:  1,247 CYC:196
C7F3  EA        NOP                             A:6F X:00 Y:00 P:6F SP:FB PPU:  1,256 CYC:199
C7F4  A9 40     LDA #$40                        A:6F X:00 Y:00 P:6F SP:FB PPU:  1,262 CYC:201
C7F6  85 01     STA $01 = FF                    A:40 X:00 Y:00 P:6D SP:FB PPU:  1,268 CYC:203
C7F8  24 01     BIT $01 = 40                    A:40 X:00 Y:00 P:6D SP:FB PPU:  1,277 CYC:206
C7FA  D8        CLD                             A:40 X:00 Y:00 P:6D SP:FB PPU:  1,286 CYC:209
C7FB  A9 10     LDA #$10                        A:40 X:00 Y:00 P:65 SP:FB PPU:  1,292 CYC:211
C7FD  18        CLC                             A:10 X:00 Y:00 P:65 SP:FB PPU:  1,298 CYC:213
C7FE  08        PHP                             A:10 X:00 Y:00 P:64 SP:FB PPU:  1,304 CYC:215
C7FF  68        PLA                             A:10 X:00 Y:00 P:64 SP:FA PPU:  1,313 CYC:218
C800  29 EF     AND #$EF                        A:74 X:00 Y:00 P:64 SP:FB PPU:  1,325 CYC:222
C802  C9 64     CMP #$64                        A:64 X:00 Y:00 P:64 SP:FB PPU:  1,331 CYC:224
C804  F0 04     BEQ $C80A                       A:64 X:00 Y:00 P:67 SP:FB PPU:  1,337 CYC:226
C80A  EA        NOP                             A:64 X:00 Y:00 P:67 SP:FB PPU:  2,  5 CYC:229
C80B  A9 80     LDA #$80                        A:64 X:00 Y:00 P:67 SP:FB PPU:  2, 11 CYC:231
C80D  85 01     STA $01 = 40                    A:80 X:00 Y:00 P:E5 SP:FB PPU:  2, 17 CYC:233
C80F  24 01     BIT $01 = 80                    A:80 X:00 Y:00 P:E5 SP:FB PPU:  2, 26 CYC:236
C811  F8        SED                             A:80 X:00 Y:00 P:A5 SP:FB PPU:  2, 35 CYC:239
C812  A9 00     LDA #$00                        A:80 X:00 Y:00 P:AD SP:FB PPU:  2, 41 CYC:241
C814  38        SEC                             A:00 X:00 Y:00 P:2F SP:FB PPU:  2, 47 CYC:243
C815  08        PHP                             A:00 X:00 Y:00 P:2F SP:FB PPU:  2, 53 CYC:245
C816  68        PLA                             A:00 X:00 Y:00 P:2F SP:FA PPU:  2, 62 CYC:248
C817  29 EF     AND #$EF                        A:3F X:00 Y:00 P:2D SP:FB PPU:  2, 74 CYC:252
        )"
    );
    richTextCtrl->MoveHome();

    button_next->Bind(wxEVT_BUTTON, &DebugWindow::OnButtonDown, this);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(richTextCtrl, 3, wxEXPAND | wxALL, 5);
    vbox->Add(button_next, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);


    SetSizer(vbox);
}

void DebugWindow::OnButtonDown(wxCommandEvent & WXUNUSED(event))
{
    static int lineIndex = 0;

    long startPos = richTextCtrl->XYToPosition(0, lineIndex); // Column 0, lineIndex (0-based)

    // Get the end position of the line
    long endPos = richTextCtrl->XYToPosition(0, lineIndex + 1);

    wxRichTextAttr attr;
        attr.SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));  // Set a monospace font
        attr.SetTextColour(*wxRED);

    richTextCtrl->SetStyle(startPos, endPos, richTextCtrl->GetDefaultStyleEx());
    richTextCtrl->ShowPosition(richTextCtrl->XYToPosition(0, ++lineIndex));


    startPos = richTextCtrl->XYToPosition(0, lineIndex); // Column 0, lineIndex (0-based)

    // Get the end position of the line
    endPos = richTextCtrl->XYToPosition(0, lineIndex + 1);

    richTextCtrl->SetStyle(startPos, endPos, attr);
}
