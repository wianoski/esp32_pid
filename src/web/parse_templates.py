import re
from os import listdir
from os.path import isfile, join


def parse_variables(line):
    p = re.compile(r'{{ (\S+) }}')
    res = p.search(line)
    output = line
    if res:
            substitution = '\" + String(' + res.group(1) + ') + \"'
            output = output.replace(res.group(0),substitution)
    return output


def parse_line(line):
    ## escape quotes
    output = line
    output = output.replace('"','\\"')

    ## Strip whitespace
    output = output.strip()
    output = parse_variables(output)

    if len(output) == 0:
        return output

    ## wrap in println
    output = 's.client.println("' + output + '");'

    return output

def parse_template(source_file, destination_file):
    with open(source_file) as s:
        lines = []
        for line in s:
            parsed = parse_line(line)
            if len(parsed):
                lines.append(parsed)

    with open(destination_file, "w") as d:
        for line in lines:
            d.write(line)
            d.write("\n")


templates = [f for f in listdir('.') if (isfile(f) and "template_" in f)]

for template in templates:
    parse_template(template, template.replace('template_',''))