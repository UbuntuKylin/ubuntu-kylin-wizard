#ifndef WIZARD_H
#define WIZARD_H

class Wizard
{
public:
    Wizard();
    ~Wizard();
    void Show();
    void fill_icon_view();
private:
    GtkBuilder *builder_;
    GtkWidget *icon_view_;
};

#endif
