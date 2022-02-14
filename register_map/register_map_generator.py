import csv
import os

import pandas

C_INT_TYPES = {
    "1:u": "uint8_t",
    "1:s": "int8_t",
    "2:u": "uint16_t",
    "2:s": "int16_t",
    "4:u": "uint32_t",
    "4:s": "int32_t",
    "8:u": "uint64_t",
    "8:s": "int64_t"
}

C_TYPEDEF = """typedef struct {
    %s
} %s;"""


def generate(excel_file, struct_name):
    dataframe = pandas.read_excel(excel_file)
    tsv_file = os.path.splitext(excel_file)[0] + ".tsv"
    dataframe.to_csv(tsv_file, sep="\t", index=None, header=True, encoding="utf8")

    py_code = []
    c_code = []

    with open(tsv_file, "r", encoding="utf8") as fp:
        reader = csv.DictReader(fp, delimiter="\t")
        for row in reader:
            if row["size"] == "":
                break

            ind = int(row["index"])
            size = int(float(row["size"]))
            dtype = row["type"]
            field = row["field"]
            descr = row["description"]

            if size == 1:
                full_descr = "%02x: %s" % (ind, descr)
            else:
                full_descr = "%02x:%02x: %s" % (ind, ind+size-1, descr)

            if dtype == "a":
                c_code.append("uint8_t %s[%d]; // %s" % (field, size, full_descr))
            else:
                int_type = C_INT_TYPES["%d:%s" % (size, dtype)]
                c_code.append("%s %s; // %s" % (int_type, field, full_descr))

            py_code.append("""Register(%d, %d, "%s")""" % (ind, size, dtype))

    print(C_TYPEDEF % ("\n    ".join(c_code), struct_name))


if __name__ == "__main__":
    generate(
        excel_file="output_struct.xlsx",
        struct_name="out_regs_t"
    )

    print("")

    generate(
        excel_file="input_struct.xlsx",
        struct_name="in_regs_t"
    )
