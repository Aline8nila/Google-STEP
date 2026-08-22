import sys
import collections

class Wikipedia:

    # Initialize the graph of pages.
    def __init__(self, pages_file, links_file):

        # A mapping from a page ID (integer) to the page title.
        # For example, self.titles[1234] returns the title of the page whose
        # ID is 1234.
        self.titles = {}

        # A set of page links.
        # For example, self.links[1234] returns an array of page IDs linked
        # from the page whose ID is 1234.
        self.links = {}

        # Read the pages file into self.titles.
        with open(pages_file) as file:
            for line in file:
                (id, title) = line.rstrip().split(" ")
                id = int(id)
                assert not id in self.titles, id
                self.titles[id] = title
                self.links[id] = []
        print("Finished reading %s" % pages_file)

        # Read the links file into self.links.
        with open(links_file) as file:
            for line in file:
                (src, dst) = line.rstrip().split(" ")
                (src, dst) = (int(src), int(dst))
                assert src in self.titles, src
                assert dst in self.titles, dst
                self.links[src].append(dst)
        print("Finished reading %s" % links_file)
        print()


    # Example: Find the longest titles.
    def find_longest_titles(self):
        titles = sorted(self.titles.values(), key=len, reverse=True)
        print("The longest titles are:")
        count = 0
        index = 0
        while count < 15 and index < len(titles):
            if titles[index].find("_") == -1:
                print(titles[index])
                count += 1
            index += 1
        print()


    # Example: Find the most linked pages.
    def find_most_linked_pages(self):
        link_count = {}
        for id in self.titles.keys():
            link_count[id] = 0

        for id in self.titles.keys():
            for dst in self.links[id]:
                link_count[dst] += 1

        print("The most linked pages are:")
        link_count_max = max(link_count.values())
        for dst in link_count.keys():
            if link_count[dst] == link_count_max:
                print(self.titles[dst], link_count_max)
        print()

    # Homework #1: Find the shortest path.
    # 'start': A title of the start page.
    # 'goal': A title of the goal page.
    def find_shortest_path(self, start, goal):
        #------------------------#
        title_to_id = {}
        for id, title in self.titles.items():
            title_to_id[title] = id
        start_id = title_to_id[start]
        goal_id = title_to_id[goal]

        # BFS
        queue = collections.deque([start_id])
        visited = set([start_id])
        prev = {start_id: None}

        # Search
        while queue:
            current = queue.popleft()

            if current == goal_id:
                break
            for next_id in self.links[current]:
                if next_id not in visited:
                    visited.add(next_id)
                    prev[next_id] = current
                    queue.append(next_id)

        # If no path
        if goal_id not in prev:
            print("No path found")
            return

        # Rebuild path backwards
        path = []
        current = goal_id

        while current is not None:
            path.append(current)
            current = prev[current]

        path.reverse()

        # Print path
        print("Shortest path:")
        for id in path:
            print(self.titles[id])
        print()
        #------------------------#
        pass

    # Homework #2: Calculate the page ranks and print the most popular pages.
    def find_most_popular_pages(self):
        #------------------------#
        n = len(self.titles)
        pagerank = {}
        for id in self.titles:
            pagerank[id] = 1.0 / n
        while True:
            new_pagerank = {}
            for id in self.titles:
                new_pagerank[id] = 0.0

            no_link_sum = 0.0

            for id in self.titles:
                if len(self.links[id]) == 0:
                    no_link_sum += pagerank[id]
                else:
                    share = pagerank[id] / len(self.links[id])
                    for dst in self.links[id]:
                        new_pagerank[dst] += share

            bonus = no_link_sum / n
            for id in self.titles:
                new_pagerank[id] += bonus

            diff = 0.0
            for id in self.titles:
                d = new_pagerank[id] - pagerank[id]
                diff += d * d

            print("sum =", sum(new_pagerank.values()), "diff =", diff)

            pagerank = new_pagerank

            if diff < 0.01:
                break

        pages = sorted(pagerank.keys(), key=lambda id: pagerank[id], reverse=True)

        print("The most popular pages are:")
        for id in pages[:10]:
            print(self.titles[id], pagerank[id])
        print()
        #------------------------#
        pass
        
if __name__ == "__main__":
    pages_file = "wikipedia_data/wikipedia_dataset/pages_medium.txt"
    links_file = "wikipedia_data/wikipedia_dataset/links_medium.txt"

    # Removed the sys.argv check as it's not suitable for Colab cell execution
    # if len(sys.argv) != 3:
    #     print("usage: %s pages_file links_file" % sys.argv[0])
    #     exit(1)

    wikipedia = Wikipedia(pages_file, links_file)
    # Example
    wikipedia.find_longest_titles()
    # Example
    wikipedia.find_most_linked_pages()
    # Homework #1
    wikipedia.find_shortest_path("渋谷", "パレートの法則")
    # Homework #2
    wikipedia.find_most_popular_pages()
    # Homework #3 (optional)
    # wikipedia.find_longest_path("渋谷", "池袋")
