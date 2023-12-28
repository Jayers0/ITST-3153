import tkinter as tk
import tkinter.scrolledtext as scrolledtext
import markdown

# Define a dictionary mapping plaintext to LaTeX
latex_mappings = {
    "ADD": "+",
    "SUB": "-",
    "MULT": "$\\cdot$",
    "DIVS": "/",
    "DIVDS": "$\\vert$",
    "MOD": "$\\mod{}$",
    "EXP": "^",
    "SQRT()": "$\\sqrt{}$",
    "ABS()": "$|x|$",
    "FACT": "!",
    "SUMB": "$\\sum$",
    "PRODB": "$\\prod$",
    "INT": "$\\int$",
    "CONINT": "$\\oint$",
    "DERIV": "$\\frac{d}{dx}$",
    "GT": ">",
    "LT": "<",
    "EQUL": "=",
    "ELM": "$\\in$",
    "SUBS": "$\\subseteq$",
    "UNION": "$\\cup$",
    "SINT": "$\\cap$",
    "SDIF": "$\\setminus$",
    "COMP": "$\\complement$",
    "SUNION": "$\\bigcup$",
    "INTR": "$\\bigcap$",
    "DOTM": "$\\circ$",
    "EMLM": "$\\odot$",
    "CARD()": "$cardinality()$",
    "FLR()": "$\\lfloor x \\rfloor$",
    "CEIL()": "$\\lceil x \\rceil$",
    "LAPLACE": "$\\mathscr{L}$",
    "HYPERT": "$\\mathscr{H}$",
    "GRAD()": "$\\nabla$",
    "curl": "$curl$",
    "land": "$\\wedge$"
}

def update_latex():
    text = text_editor.get("1.0", "end-1c")
    for plain_text, latex_code in latex_mappings.items():
        if plain_text in text:
            text = text.replace(plain_text, latex_code)
    # Ensure 'latex_editor' is defined before using it
    if 'latex_editor' in globals():
        latex_editor.delete("1.0", "end")
        latex_editor.insert("1.0", text)
    # Schedule the update_latex function to run again after 500 milliseconds (half a second)
    root.after(500, update_latex)

def render_markdown():
    markdown_text = text_editor.get("1.0", "end-1c")
    html_text = markdown.markdown(markdown_text)
    preview_label.config(text=html_text)

def save_to_file():
    markdown_content = text_editor.get("1.0", "end-1c")
    with open("output.md", "w") as file:
        file.write(markdown_content)

def save_latex_to_file():
    latex_content = latex_editor.get("1.0", "end-1c")
    with open("output.tex", "w") as file:
        file.write(latex_content)

# Create the main window
root = tk.Tk()
root.title("Markdown and LaTeX Editor")

# Markdown editor
text_editor = scrolledtext.ScrolledText(root, wrap=tk.WORD, height=20, width=50)
text_editor.pack(pady=10, padx=10)

# LaTeX editor
latex_editor = scrolledtext.ScrolledText(root, wrap=tk.WORD, height=20, width=50)
latex_editor.pack(pady=10, padx=10)

# Text analysis labels
word_count_label = tk.Label(root, text="Words: 0")
char_count_label = tk.Label(root, text="Characters: 0")
paragraph_count_label = tk.Label(root, text="Paragraphs: 0")
word_count_label.pack()
char_count_label.pack()
paragraph_count_label.pack()

# Markdown preview label
preview_label = tk.Label(root, text="Markdown Preview")
preview_label.pack()

# Buttons
update_button = tk.Button(root, text="Update LaTeX", command=update_latex)
render_button = tk.Button(root, text="Render Markdown", command=render_markdown)
save_button = tk.Button(root, text="Save Markdown to File", command=save_to_file)
save_latex_button = tk.Button(root, text="Save LaTeX to File", command=save_latex_to_file)

update_button.pack()
render_button.pack()
save_button.pack()
save_latex_button.pack()

# Schedule the initial call to update_latex
root.after(500, update_latex)

# Start the main loop
root.mainloop()