from fpdf import FPDF

class PDF(FPDF):
    def __init__(self, header_logo):
        super().__init__()
        self.WIDTH = 210
        self.HEIGHT = 297
        self.header_logo = header_logo
        self.default_font = 'Arial'

        # Header and footer


    def header(self):
        # Custom logo and positioning
        # Create an `assets` folder and put any wide and short image inside
        # Name the image `logo.png`
        self.image(self.header_logo, 10, 8, 33, 8, type = 'png')
        self.set_font(self.default_font, 'B', 11)
        self.cell(self.WIDTH - 80)
        self.cell(60, 1, 'Monthly Report', 0, 0, 'R')
        self.ln(20)

    def set_title(self):

        self.set_font(self.default_font, 'B', 28)
        self.cell(w=0, h=15, txt="Monthly Report", align="C")
        self.set_font(self.default_font, 'B', 11)

    def set_text(self, text = "This is just to experiment with the text. Wonder what happens if I add. \nDoes anything happen?"):

        self.set_font(self.default_font, 'B', 28)
        self.multi_cell(w = 0, h = 10, txt=text, align="C")
        self.set_font(self.default_font, 'B', 11)

    def footer(self):
        # Page numbers in the footer
        self.set_y(-15)
        self.set_font('Arial', 'I', 8)
        self.set_text_color(128)
        self.cell(0, 10, 'Page ' + str(self.page_no()), 0, 0, 'C')

    def page_body(self, images):
        # Determine how many plots there are per page and set positions
        # and margins accordingly
        if len(images) == 3:
            self.image(images[0], 15, 25, self.WIDTH - 30)
            self.image(images[1], 15, self.WIDTH / 2 + 5, self.WIDTH - 30)
            self.image(images[2], 15, self.WIDTH / 2 + 90, self.WIDTH - 30)
        elif len(images) == 2:
            self.image(images[0], 15, 25, self.WIDTH - 30)
            self.image(images[1], 15, self.WIDTH / 2 + 5, self.WIDTH - 30)
        else:
            self.image(images[0], 15, 25, self.WIDTH - 30, type = "PNG")

    def print_page(self, images):
        # Generates the report
        self.add_page()
        self.set_title()
        self.set_text()
        self.page_body(images)